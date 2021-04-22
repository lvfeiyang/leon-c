#pragma once

#include <folly/AtomicUnorderedMap.h>
#include <folly/SocketAddress.h>
#include <folly/container/F14Map.h>
#include <folly/io/SocketOptionMap.h>
#include <folly/io/async/EventBase.h>
#include <folly/io/async/HHWheelTimer.h>
#include <proxygen/lib/http/HTTPConnector.h>

#include "curl/CurlClient.h"

namespace HttpCli {

static const folly::SocketOptionMap opts{{{SOL_SOCKET, SO_REUSEADDR}, 1}};

class CliManage {
 public:
  explicit CliManage(std::string addrUrl, size_t num);
  ~CliManage();

  void send(std::string method, std::string url, std::string head,
            std::string req, std::string* rsp);

 private:
  struct connResource {
    std::unique_ptr<CurlService::CurlClient> client;
    std::unique_ptr<proxygen::HTTPConnector> connector;
    folly::EventBase evb;
  };
  std::unique_ptr<folly::SocketAddress> addr_;
  folly::fbvector<std::unique_ptr<connResource>> cliConns_;
  size_t cliNum_;
};

class CliManageFactory {
 public:
  explicit CliManageFactory();
  ~CliManageFactory();
  static CliManageFactory& singleton();

  enum class AddrOp { Add, Del };

  bool addCliManage(std::string name, std::string addrUrl, size_t num);
  bool cliChgAddr(AddrOp op, std::string name, std::string addr);
  folly::HHWheelTimer* getTimer();
  CliManage* getClient(std::string name);

 private:
  folly::EventBase evb_;
  folly::HHWheelTimer::UniquePtr timer_;
  folly::F14FastMap<folly::fbstring,
                    folly::fbvector<std::unique_ptr<CliManage>>>
      cliMap_;
};
}  // namespace HttpCli