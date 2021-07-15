#define _TURN_OFF_PLATFORM_STRING

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "client/HttpClient.h"
#include "util/pbdata/serv-msg.pb.h"

TEST(module_curl, test_win_rest) {
  using namespace utility;    // Common utilities like string conversions
  using namespace web;        // Common features like URIs.
  using namespace web::http;  // Common HTTP functionality
  using namespace web::http::client;     // HTTP client features
  using namespace concurrency::streams;  // Asynchronous streams

  std::string reqMsgData;
  ServMsg::PullReq req;
  req.add_keys(11);
  req.add_keys(44);
  req.add_field(1);
  req.add_field(2);
  req.SerializeToString(&reqMsgData);

  http_client client(_XPLATSTR("http://127.0.0.1:11007/"));
  uri_builder builder(_XPLATSTR("/pull"));
  pplx::task<void> requestTask =
      client.request(methods::POST, builder.to_string(), reqMsgData)
          .then([](http_response response) {
            printf("Received response status code:%u body:%s\n",
                   response.status_code(), response.to_string().data());
            return;
          });
  pplx::task<void> requestTask1 =
      client.request(methods::POST, builder.to_string(), reqMsgData)
          .then([](http_response response) {
            printf("Received response 1 status code:%u body:%s\n",
                   response.status_code(), response.to_string().data());
            return;
          });
  try {
    requestTask.wait();
    requestTask1.wait();
  } catch (const std::exception& e) {
    printf("Error exception:%s\n", e.what());
  }
  LOG(INFO) << "leon_debug 1";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

TEST(module_curl, test_newcli) {
  HttpClient::CliManageFactory::singleton().addCliManage(
      "leon_test", "http://127.0.0.1:11007", 1);

  // std::this_thread::sleep_for(std::chrono::milliseconds(50));
  // std::thread a([]()
  {
    std::string reqMsgData;
    std::string rspMsgData;
    folly::F14FastMap<std::string, std::string> rspHead;

    ServMsg::PullReq req;
    req.add_keys(11);
    req.add_keys(44);
    req.add_field(1);
    req.add_field(2);
    req.SerializeToString(&reqMsgData);

    HttpClient::CliManageFactory::singleton()
        .getClient("leon_test")
        ->send("POST", "/pull", "", reqMsgData, rspMsgData, rspHead);
    LOG(INFO) << "leon_debug test send";

    ServMsg::PullRsp rsp;
    rsp.ParseFromString(rspMsgData);

    LOG(INFO) << "leon_debug " << rsp.DebugString() << ";" << rspMsgData.size();
    for (auto iter = rspHead.begin(); iter != rspHead.end(); iter++) {
      LOG(INFO) << "leon_debug rsp head " << iter->first << ": "
                << iter->second;
    }
  }
  //);

  // twice
  // std::thread b([]()
  {
    std::string reqMsgData;
    std::string rspMsgData;
    folly::F14FastMap<std::string, std::string> rspHead;

    ServMsg::PullReq req;
    req.add_keys(11);
    req.add_keys(44);
    req.add_field(1);
    req.add_field(2);
    req.SerializeToString(&reqMsgData);

    HttpClient::CliManageFactory::singleton()
        .getClient("leon_test")
        ->send("POST", "/pull", "", reqMsgData, rspMsgData, rspHead);
    LOG(INFO) << "leon_debug test send 2";

    ServMsg::PullRsp rsp;
    rsp.ParseFromString(rspMsgData);

    LOG(INFO) << "leon_debug 2 " << rsp.DebugString() << ";"
              << rspMsgData.size();
    for (auto iter = rspHead.begin(); iter != rspHead.end(); iter++) {
      LOG(INFO) << "leon_debug 2 rsp head " << iter->first << ": "
                << iter->second;
    }
  }
  // );

  // a.join();
  // b.join();
}