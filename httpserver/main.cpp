#include <proxygen/httpserver/HTTPServer.h>

#include <iostream>

#include "service/Service.h"

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_string(ip, "localhost", "IP/Hostname to bind to");
DEFINE_int32(threads, 0,
             "Number of threads to listen on. Numbers <= 0 "
             "will use the number of cores on this machine.");

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  std::vector<proxygen::HTTPServer::IPConfig> IPs = {
      {folly::SocketAddress(FLAGS_ip, FLAGS_http_port, true),
       proxygen::HTTPServer::Protocol::HTTP},
  };
  if (FLAGS_threads <= 0) {
    FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
    CHECK(FLAGS_threads > 0);
    // LOG(INFO) << FLAGS_threads;
  }

  proxygen::HTTPServerOptions options;
  options.threads = static_cast<size_t>(FLAGS_threads);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.handlerFactories = proxygen::RequestHandlerChain()
                                 .addThen<HService::FsHandlerFactory>()
                                 .build();
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