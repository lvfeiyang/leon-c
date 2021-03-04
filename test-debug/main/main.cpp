#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/httpserver/ResponseBuilder.h>

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_string(ip, "localhost", "IP/Hostname to bind to");
DEFINE_int32(threads, 0,
             "Number of threads to listen on. Numbers <= 0 "
             "will use the number of cores on this machine.");

class TestHandler : public proxygen::RequestHandler {
 public:
  explicit TestHandler(){};

  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override {
    // stats_->recordRequest();
    proxygen::ResponseBuilder builder(downstream_);
    builder.status(200, "OK");
    headers->getHeaders().forEach([&](std::string& name, std::string& value) {
      builder.header(folly::to<std::string>("x-echo-", name), value);
    });
    LOG(INFO) << headers->getMethodString() << headers->getURL()
              << headers->getPath() << headers->getQueryString();
    builder.send();
  };

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    LOG(INFO) << "leon debug on body";
    proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
    // proxygen::ResponseBuilder(downstream_).body("hello\n").send();
  };

  void onEOM() noexcept override {
    proxygen::ResponseBuilder(downstream_).sendWithEOM();
  };

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override{};

  void requestComplete() noexcept override { delete this; };

  void onError(proxygen::ProxygenError err) noexcept override { delete this; };

 private:
  // EchoStats* const stats_{nullptr};

  std::unique_ptr<folly::IOBuf> body_;
};

class Test1Handler : public proxygen::RequestHandler {
 public:
  explicit Test1Handler(){};

  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override {
    // stats_->recordRequest();
    proxygen::ResponseBuilder builder(downstream_);
    builder.status(200, "OK");
    // headers->getHeaders().forEach([&](std::string& name, std::string& value)
    // {
    //   builder.header(folly::to<std::string>("x-echo-", name), value);
    // });
    builder.send();
  };

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    // proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
    proxygen::ResponseBuilder(downstream_).body("hello\n").send();
  };

  void onEOM() noexcept override {
    proxygen::ResponseBuilder(downstream_).sendWithEOM();
  };

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override{};

  void requestComplete() noexcept override { delete this; };

  void onError(proxygen::ProxygenError err) noexcept override { delete this; };
};

class TestHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* /*evb*/) noexcept override {
    // stats_.reset(new EchoStats);
  }

  void onServerStop() noexcept override {
    // stats_.reset();
  }

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler*,
      proxygen::HTTPMessage* head) noexcept override {
    if (head->getURL() == "/test1") {
      return new Test1Handler();
    } else {
      return new TestHandler();
    }
  }

  //  private:
  //   folly::ThreadLocalPtr<EchoStats> stats_;
};

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  LOG(INFO) << "leon debug begin";

  std::vector<proxygen::HTTPServer::IPConfig> IPs = {
      {folly::SocketAddress(FLAGS_ip, FLAGS_http_port, true),
       proxygen::HTTPServer::Protocol::HTTP},
  };
  if (FLAGS_threads <= 0) {
    FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
    CHECK(FLAGS_threads > 0);
  }

  proxygen::HTTPServerOptions options;
  options.threads = static_cast<size_t>(FLAGS_threads);
  // options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  // options.enableContentCompression = false;
  options.handlerFactories =
      proxygen::RequestHandlerChain().addThen<TestHandlerFactory>().build();
  // Increase the default flow control to 1MB/10MB
  options.initialReceiveWindow = uint32_t(1 << 20);
  options.receiveStreamWindowSize = uint32_t(1 << 20);
  options.receiveSessionWindowSize = 10 * (1 << 20);

  proxygen::HTTPServer server(std::move(options));
  server.bind(IPs);

  // Start HTTPServer mainloop in a separate thread
  std::thread t([&]() { server.start(); });

  t.join();
  return 0;
}
