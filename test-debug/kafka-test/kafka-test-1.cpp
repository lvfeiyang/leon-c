#include <librdkafka/rdkafkacpp.h>
#include <sys/time.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

static volatile sig_atomic_t run = 1;
static bool exit_eof = false;
static int eof_cnt = 0;
static int partition_cnt = 0;
static int verbosity = 1;
static long msg_cnt = 0;
static int64_t msg_bytes = 0;
static void sigterm(int sig) { run = 0; }

static void print_time() {
#ifndef _WIN32
  struct timeval tv;
  char buf[64];
  gettimeofday(&tv, NULL);
  strftime(buf, sizeof(buf) - 1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
  fprintf(stderr, "%s.%03d: ", buf, (int)(tv.tv_usec / 1000));
#else
  SYSTEMTIME lt = {0};
  GetLocalTime(&lt);
  // %Y-%m-%d %H:%M:%S.xxx:
  fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d.%03d: ", lt.wYear, lt.wMonth,
          lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
#endif
}

class ExampleEventCb : public RdKafka::EventCb {
 public:
  void event_cb(RdKafka::Event &event) {
    // print_time();

    switch (event.type()) {
      case RdKafka::Event::EVENT_ERROR:
        // if (event.fatal()) {
        //   std::cerr << "FATAL ";
        //   run = 0;
        // }
        std::cerr << "ERROR (" << RdKafka::err2str(event.err())
                  << "): " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_STATS:
        std::cerr << "\"STATS\": " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_LOG:
        // fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(),
        //         event.fac().c_str(), event.str().c_str());
        break;

      case RdKafka::Event::EVENT_THROTTLE:
        std::cerr << "THROTTLED: " << event.throttle_time() << "ms by "
                  << event.broker_name() << " id " << (int)event.broker_id()
                  << std::endl;
        break;

      default:
        std::cerr << "EVENT " << event.type() << " ("
                  << RdKafka::err2str(event.err()) << "): " << event.str()
                  << std::endl;
        break;
    }
  }
};
class ExampleRebalanceCb : public RdKafka::RebalanceCb {
 private:
  static void part_list_print(
      const std::vector<RdKafka::TopicPartition *> &partitions) {
    for (unsigned int i = 0; i < partitions.size(); i++)
      std::cerr << partitions[i]->topic() << "[" << partitions[i]->partition()
                << "], ";
    std::cerr << "\n";
  }

 public:
  void rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err,
                    std::vector<RdKafka::TopicPartition *> &partitions) {
    std::cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";

    part_list_print(partitions);

    RdKafka::ErrorCode ret_err = RdKafka::ERR_NO_ERROR;

    if (err == RdKafka::ERR__ASSIGN_PARTITIONS) {
      ret_err = consumer->assign(partitions);
      partition_cnt += (int)partitions.size();
    } else {
      ret_err = consumer->unassign();
      partition_cnt = 0;
    }
    eof_cnt = 0; /* FIXME: Won't work with COOPERATIVE */

    if (ret_err)
      std::cerr << "assign failed: " << RdKafka::err2str(ret_err) << "\n";
  }
};
void msg_consume(RdKafka::Message *message, void *opaque) {
  switch (message->err()) {
    case RdKafka::ERR__TIMED_OUT:
      break;

    case RdKafka::ERR_NO_ERROR:
      /* Real message */
      msg_cnt++;
      msg_bytes += message->len();
      if (verbosity >= 3)
        std::cerr << "Read msg at offset " << message->offset() << std::endl;
      RdKafka::MessageTimestamp ts;
      ts = message->timestamp();
      if (verbosity >= 2 &&
          ts.type != RdKafka::MessageTimestamp::MSG_TIMESTAMP_NOT_AVAILABLE) {
        std::string tsname = "?";
        if (ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_CREATE_TIME)
          tsname = "create time";
        else if (ts.type ==
                 RdKafka::MessageTimestamp::MSG_TIMESTAMP_LOG_APPEND_TIME)
          tsname = "log append time";
        std::cout << "Timestamp: " << tsname << " " << ts.timestamp
                  << std::endl;
      }
      if (verbosity >= 2 && message->key()) {
        std::cout << "Key: " << *message->key() << std::endl;
      }
      if (verbosity >= 1) {
        std::cout << "leon_debug get content " << static_cast<int>(message->len())
                  << std::endl;
        // printf("%.*s\n", static_cast<int>(message->len()),
        //        static_cast<const char *>(message->payload()));
      }
      break;

    case RdKafka::ERR__PARTITION_EOF:
      /* Last message */
      if (exit_eof && ++eof_cnt == partition_cnt) {
        std::cerr << "%% EOF reached for all " << partition_cnt
                  << " partition(s)" << std::endl;
        run = 0;
      }
      break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = 0;
      break;

    default:
      /* Errors */
      std::cerr << "Consume failed: " << message->errstr() << std::endl;
      run = 0;
  }
};

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  std::string brokers = "di-inno-kafka01.h.ab1.qttsite.net:9092";
  std::string errstr;
  std::string topic_str;
  std::string mode;
  std::string debug;
  std::vector<std::string> topics;

  /*
   * Create configuration objects
   */
  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

  ExampleRebalanceCb ex_rebalance_cb;
  conf->set("rebalance_cb", &ex_rebalance_cb, errstr);

  conf->set("enable.partition.eof", "true", errstr);

  if (conf->set("group.id", "leon_test", errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }
  if (conf->set("compression.codec", "lz4", errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }
  exit_eof = true;
  debug = "all";
  if (conf->set("statistics.interval.ms", "1000", errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }

  /* Try "topic." prefixed properties on topic
   * conf first, and then fall through to global if
   * it didnt match a topic configuration property. */
  // RdKafka::Conf::ConfResult res = RdKafka::Conf::CONF_UNKNOWN;
  // res = tconf->set(name + strlen("topic."), val, errstr);
  // res = conf->set(name, val, errstr);
  // if (res != RdKafka::Conf::CONF_OK) {
  //   std::cerr << errstr << std::endl;
  //   exit(1);
  // }
  if (tconf->set("auto.offset.reset", "smallest", errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(1);
  }

  // verbosity--;  // -q
  verbosity++;  // -v

  topics.push_back("mt_predict_dump");

  if (exit_eof) {
    std::string strategy;
    if (conf->get("partition.assignment.strategy", strategy) ==
            RdKafka::Conf::CONF_OK &&
        strategy == "cooperative-sticky") {
      std::cerr
          << "Error: this example has not been modified to "
          << "support -e (exit on EOF) when the partition.assignment.strategy "
          << "is set to " << strategy << ": remove -e from the command line\n";
      exit(1);
    }
  }

  /*
   * Set configuration properties
   */
  conf->set("metadata.broker.list", brokers, errstr);

  if (!debug.empty()) {
    if (conf->set("debug", debug, errstr) != RdKafka::Conf::CONF_OK) {
      std::cerr << errstr << std::endl;
      exit(1);
    }
  }

  ExampleEventCb ex_event_cb;
  conf->set("event_cb", &ex_event_cb, errstr);

  conf->set("default_topic_conf", tconf, errstr);
  delete tconf;

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);

  /*
   * Consumer mode
   */

  /*
   * Create consumer using accumulated global configuration.
   */
  RdKafka::KafkaConsumer *consumer =
      RdKafka::KafkaConsumer::create(conf, errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << errstr << std::endl;
    exit(1);
  }

  delete conf;

  std::cout << "% Created consumer " << consumer->name() << std::endl;

  /*
   * Subscribe to topics
   */
  RdKafka::ErrorCode err = consumer->subscribe(topics);
  if (err) {
    std::cerr << "Failed to subscribe to " << topics.size()
              << " topics: " << RdKafka::err2str(err) << std::endl;
    exit(1);
  }

  /*
   * Consume messages
   */
  int count = 0;
  while (run) {
    RdKafka::Message *msg = consumer->consume(1000);
    msg_consume(msg, NULL);
    delete msg;
    if (count++ > 10) run = 0;
  }

#ifndef _WIN32
  alarm(10);
#endif

  /*
   * Stop consumer
   */
  consumer->close();
  delete consumer;

  std::cerr << "% Consumed " << msg_cnt << " messages (" << msg_bytes
            << " bytes)" << std::endl;

  /*
   * Wait for RdKafka to decommission.
   * This is not strictly needed (with check outq_len() above), but
   * allows RdKafka to clean up all its resources before the application
   * exits so that memory profilers such as valgrind wont complain about
   * memory leaks.
   */
  RdKafka::wait_destroyed(5000);

  return 0;
}