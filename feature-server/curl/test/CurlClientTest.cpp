// #include <folly/File.h>
#include <folly/SocketAddress.h>
// #include <folly/io/RecordIO.h>
#include <folly/io/SocketOptionMap.h>
#include <folly/io/async/AsyncTimeout.h>
#include <folly/io/async/EventBase.h>
#include <folly/io/async/HHWheelTimer.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <proxygen/lib/http/HTTPConnector.h>
#include <proxygen/lib/http/HTTPHeaders.h>
#include <proxygen/lib/utils/URL.h>

#include "curl/CurlClient.h"
#include "util/pbdata/serv-msg.pb.h"

TEST(module_curl, test_get) {
  // folly::File f("test-req.txt", folly::O_RDWR);
  // folly::RecordIOWriter writer(f);
  std::ofstream f;
  f.open("test-req.txt", std::ios::out);

  // f << "leon_test";
  ServMsg::PullReq a1;
  a1.add_keys(11);
  a1.add_keys(44);
  a1.add_field(1);
  a1.add_field(2);
  a1.SerializeToOstream((std::ostream*)&f);

  f.close();
  // return;

  // std::streambuf sbuf;
  std::istream rspStream(std::cout.rdbuf());
  folly::EventBase evb;
  proxygen::URL url("http://127.0.0.1:11000/");
  proxygen::HTTPHeaders headers =
      CurlService::CurlClient::parseHeaders("a1=b1,a2=b2");
  CurlService::CurlClient curlClient(&evb, proxygen::HTTPMethod::POST, url,
                                     nullptr, headers, "test-req.txt", true);
  curlClient.setFlowControlSettings(65536);
  curlClient.setLogging(true);
  // curlClient.saveResponseToPB(rspStream.rdbuf());
  curlClient.saveResponseToFile("test-rsp.txt");

  folly::SocketAddress addr =
      folly::SocketAddress(url.getHost(), url.getPort(), true);
  LOG(INFO) << "Trying to connect to " << addr;

  folly::HHWheelTimer::UniquePtr timer{folly::HHWheelTimer::newTimer(
      &evb,
      std::chrono::milliseconds(folly::HHWheelTimer::DEFAULT_TICK_INTERVAL),
      folly::AsyncTimeout::InternalEnum::NORMAL,
      std::chrono::milliseconds(5000))};
  proxygen::HTTPConnector connector(&curlClient, timer.get());
  static const folly::SocketOptionMap opts{{{SOL_SOCKET, SO_REUSEADDR}, 1}};
  connector.connect(&evb, addr, std::chrono::milliseconds(1000), opts);
  evb.loop();

  std::ifstream f1;
  f1.open("test-rsp.txt", std::ios::in);
  ServMsg::PullRsp rsp;
  rsp.ParseFromIstream((std::istream*)&f1);
  LOG(INFO) << "leon_debug " << rsp.DebugString();
}
