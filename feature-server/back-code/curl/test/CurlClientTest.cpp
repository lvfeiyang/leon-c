// #include <folly/File.h>
#include <folly/SocketAddress.h>
// #include <folly/io/RecordIO.h>
#include <folly/dynamic.h>
#include <folly/io/SocketOptionMap.h>
#include <folly/io/async/AsyncTimeout.h>
#include <folly/io/async/EventBase.h>
#include <folly/io/async/HHWheelTimer.h>
#include <folly/json.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <proxygen/lib/http/HTTPConnector.h>
#include <proxygen/lib/http/HTTPHeaders.h>
#include <proxygen/lib/utils/URL.h>

#include "curl/CurlClient.h"
#include "curl/HttpCli.h"
#include "util/pbdata/serv-msg.pb.h"

TEST(module_curl, test_newcli) {
  // folly::init(nullptr, nullptr);

  HttpCli::CliManageFactory::singleton().addCliManage(
      "leon_test", "http://127.0.0.1:11007", 2);

  std::string reqMsgData;
  std::string rspMsgData;
  folly::F14FastMap<std::string, std::string> rspHead;

  ServMsg::PullReq req;
  req.add_keys(11);
  req.add_keys(44);
  req.add_field(1);
  req.add_field(2);
  req.SerializeToString(&reqMsgData);

  HttpCli::CliManageFactory::singleton()
      .getClient("leon_test")
      ->send("POST", "/pull", "", reqMsgData, &rspMsgData, rspHead);

  ServMsg::PullRsp rsp;
  rsp.ParseFromString(rspMsgData);

  LOG(INFO) << "leon_debug " << rsp.DebugString() << ";" << rspMsgData.size();
}

TEST(module_consul, test_register) {
  HttpCli::CliManageFactory::singleton().addCliManage(
      "leon_consul", "http://registry-qa.qutoutiao.net", 1);

  std::string reqMsgData;
  std::string rspMsgData;
  folly::F14FastMap<std::string, std::string> rspHead;

  // folly::dynamic req = folly::dynamic::object("Name", "leon_fs_test")(
  //     "ID", "leon_fs_test_1")("Tags", folly::dynamic::array("leon", "test"))(
  //     "Address", "127.0.0.1")("Port", 9977)("EnableTagOverride", false)(
  //     "Check",
  //     folly::dynamic::object("DeregisterCriticalServiceAfter", "90m")(
  //         "Interval", "10s")("Timeout", "5s")("HTTP",
  //         "https://example.com"));
  // reqMsgData = folly::toJson(req);
  // LOG(INFO) << "leon_debug req " << reqMsgData;

  HttpCli::CliManageFactory::singleton()
      .getClient("leon_consul")
      ->send("GET", "/v1/health/service/leon_fs_test",
             "Content-Type=application/json", "", &rspMsgData, rspHead);

  for (auto kv : rspHead) {
    LOG(INFO) << kv.first << ": " << kv.second;
  }
  folly::dynamic rsp = folly::parseJson(rspMsgData);

  LOG(INFO) << "leon_debug rsp " << rsp;

  rspMsgData.clear();
  rspHead.clear();
  HttpCli::CliManageFactory::singleton()
      .getClient("leon_consul")
      ->send("GET", "/v1/health/service/leon_fs_test",
             "Content-Type=application/json,X-Consul-Index=70814136", "",
             &rspMsgData, rspHead);
  LOG(INFO) << "leon_debug rsp 2 " << rsp;
}

/*TEST(module_curl, test_httpcli) {
  std::string reqMsgData;
  std::string rspMsgData;

  ServMsg::PullReq req;
  req.add_keys(11);
  req.add_keys(44);
  req.add_field(1);
  req.add_field(2);
  req.SerializeToString(&reqMsgData);

  auto cliMng = HttpCli::CliManage("http://127.0.0.1:11007/pull", "");
  cliMng.send(std::move(reqMsgData), &rspMsgData);

  ServMsg::PullRsp rsp;
  rsp.ParseFromString(rspMsgData);

  LOG(INFO) << "leon_debug " << rsp.DebugString() << ";" << rspMsgData.size();
}
TEST(module_curl, test_push) {
  std::string reqMsgData;
  std::string rspMsgData;

  ServMsg::PushReq req;
  ServMsg::oneInfo* info = req.add_infos();
  info->set_key(44);
  info->set_data("test_1");
  req.SerializeToString(&reqMsgData);

  auto cliMng = HttpCli::CliManage("http://127.0.0.1:11007/push", "");
  cliMng.send(std::move(reqMsgData), &rspMsgData);

  ServMsg::PullRsp rsp;
  rsp.ParseFromString(rspMsgData);

  LOG(INFO) << "leon_debug " << rsp.DebugString();
}
TEST(module_curl, test_delcache) {
  std::string reqMsgData;
  std::string rspMsgData;

  ServMsg::DelCacheReq req;
  req.set_key(11);
  req.SerializeToString(&reqMsgData);

  auto cliMng = HttpCli::CliManage("http://127.0.0.1:11007/delcache", "");
  cliMng.send(std::move(reqMsgData), &rspMsgData);

  ServMsg::DelCacheRsp rsp;
  rsp.ParseFromString(rspMsgData);

  LOG(INFO) << "leon_debug " << rsp.DebugString();
}

TEST(module_consul, test_register) {
  std::string reqMsgData;
  std::string rspMsgData;

  folly::dynamic req = folly::dynamic::object("Name", "leon_fs_test")(
      "ID", "leon_fs_test_1")("Tags", folly::dynamic::array("leon", "test"))(
      "Address", "127.0.0.1")("Port", 9977)("EnableTagOverride", false)(
      "Check",
      folly::dynamic::object("DeregisterCriticalServiceAfter", "90m")(
          "Interval", "10s")("Timeout", "5s")("HTTP", "https://example.com"));
  reqMsgData = folly::toJson(req);
  LOG(INFO) << "leon_debug req " << reqMsgData;

  auto cliMng = HttpCli::CliManage(
      "http://registry-qa.qutoutiao.net/v1/agent/service/register",
      "Content-Type=application/json,Content-Length=" +
          folly::to<std::string>(reqMsgData.size()));
  cliMng.send(std::move(reqMsgData), &rspMsgData);

  LOG(INFO) << "leon_debug rsp " << rspMsgData;
}

TEST(module_curl, test_get) {
  // folly::File f("test-req.txt", folly::O_RDWR);
  // folly::RecordIOWriter writer(f);
  std::ofstream f;
  f.open("test-req.txt", std::ios::out);

  folly::dynamic req = folly::dynamic::object("Name", "leon_fs_test")(
      "ID", "leon_fs_test_1")("Tags", folly::dynamic::array("leon", "test"))(
      "Address", "127.0.0.1")("Port", 9977)("EnableTagOverride", false)(
      "Check",
      folly::dynamic::object("DeregisterCriticalServiceAfter", "90m")(
          "Interval", "10s")("Timeout", "5s")("HTTP", "https://example.com"));
  f << folly::toJson(req);

  // f << "leon_test";
  // ServMsg::PullReq a1;
  // a1.add_keys(11);
  // a1.add_keys(44);
  // a1.add_field(1);
  // a1.add_field(2);
  // // a1.SerializeToOstream((std::ostream*)&f);
  // std::string reqMsgData;
  // a1.SerializeToString(&reqMsgData);

  f.close();
  // return;

  // std::streambuf sbuf;
  std::istream rspStream(std::cout.rdbuf());
  folly::EventBase evb;
  proxygen::URL url(
      "http://registry-qa.qutoutiao.net/v1/agent/service/register");
  proxygen::HTTPHeaders headers = CurlService::CurlClient::parseHeaders(
      "a1=b1,a2=b2,Content-Type=application/json");
  CurlService::CurlClient curlClient(&evb, proxygen::HTTPMethod::PUT, url,
                                     nullptr, headers, "test-req.txt", true);
  curlClient.setFlowControlSettings(65536);
  curlClient.setLogging(true);
  // curlClient.saveResponseToPB(rspStream.rdbuf());
  curlClient.saveResponseToFile("test-rsp.txt");
  // curlClient.setSendMsg(std::move(reqMsgData));
  std::string rspMsgData;
  curlClient.setRespMsg(&rspMsgData);

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

  return;

  std::ifstream f1;
  f1.open("test-rsp.txt", std::ios::in);
  ServMsg::PullRsp rsp;
  // rsp.ParseFromIstream((std::istream*)&f1);
  rsp.ParseFromString(rspMsgData);
  f1.close();

  LOG(INFO) << "leon_debug " << rsp.DebugString() << ";" << rspMsgData.size();

  ServMsg::PullReq a2;
  a2.add_keys(11);
  a2.add_keys(55);
  a2.add_field(2);
  a2.add_field(3);
  std::string reqMsgData1;
  a2.SerializeToString(&reqMsgData1);
  curlClient.setSendMsg(std::move(reqMsgData1));
  std::string rspMsgData1;
  curlClient.setRespMsg(&rspMsgData1);

  connector.connect(&evb, addr, std::chrono::milliseconds(1000), opts);
  evb.loop();

  ServMsg::PullRsp rsp1;
  rsp1.ParseFromString(rspMsgData1);
  LOG(INFO) << "leon_debug " << rsp1.DebugString();
}*/

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  folly::init(&argc, &argv);

  return RUN_ALL_TESTS();
}
