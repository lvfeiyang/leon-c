// https://github.com/edenhill/librdkafka/blob/master/examples/rdkafka_complex_consumer_example.cpp

#include <librdkafka/rdkafka.h>
#include <librdkafka/rdkafkacpp.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  char hostname[128];
  char errstr[512];
  if (gethostname(hostname, sizeof(hostname))) {
    fprintf(stderr, "%% Failed to lookup hostname\n");
    exit(1);
  }
  RdKafka::Conf *confCpp = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  std::cout << (confCpp == NULL) << "hostname: " << hostname << std::endl;

  rd_kafka_conf_t *conf = rd_kafka_conf_new();
  if (rd_kafka_conf_set(conf, "client.id", hostname, errstr, sizeof(errstr)) !=
      RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%% %s\n", errstr);
    exit(1);
  }
  if (rd_kafka_conf_set(conf, "group.id", "leon-cpp-test", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%% %s\n", errstr);
    exit(1);
  }
  if (rd_kafka_conf_set(conf, "bootstrap.servers", "host1:9092,host2:9092",
                        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%% %s\n", errstr);
    exit(1);
  }

  /* Create Kafka consumer handle */
  rd_kafka_t *rk;
  if (!(rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr)))) {
    fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
    exit(1);
  }

  rd_kafka_resp_err_t err;
  // if ((err = rd_kafka_subscribe(rk, topics))) {
  //   fprintf(stderr, "%% Failed to start consuming topics: %s\n",
  //           rd_kafka_err2str(err));
  //   exit(1);
  // }

  int msg_count = 0;
  while (msg_count < 20) {
    rd_kafka_message_t *rkmessage = rd_kafka_consumer_poll(rk, 500);
    if (rkmessage) {
      // msg_process(rkmessage);
      std::cout << rkmessage->len << std::endl;
      rd_kafka_message_destroy(rkmessage);

      if ((++msg_count % 1000) == 0) rd_kafka_commit(rk, NULL, 0);
    }
  }

  err = rd_kafka_consumer_close(rk);
  if (err)
    fprintf(stderr, "%% Failed to close consumer: %s\n", rd_kafka_err2str(err));
  else
    fprintf(stderr, "%% Consumer closed\n");

  return 0;
}
