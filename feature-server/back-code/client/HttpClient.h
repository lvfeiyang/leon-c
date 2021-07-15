#pragma once

#include <folly/executors/FiberIOExecutor.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/futures/Future.h>
#include <proxygen/lib/http/HTTPConnector.h>
#include <proxygen/lib/http/session/HTTPTransaction.h>
#include <proxygen/lib/http/session/HTTPUpstreamSession.h>
#include <proxygen/lib/utils/URL.h>

namespace HttpClient {

static const folly::SocketOptionMap opts{{{SOL_SOCKET, SO_REUSEADDR}, 1}};

class CliSession : public proxygen::HTTPConnector::Callback {
 public:
  explicit CliSession(const std::string& addrurl);
  ~CliSession() = default;
  CliSession(CliSession&& clisess);

  // HTTPConnector methods
  void connectSuccess(proxygen::HTTPUpstreamSession* session) override;
  void connectError(const folly::AsyncSocketException& ex) override;

  // self get
  folly::Optional<proxygen::HTTPUpstreamSession*> getSession();

 private:
  proxygen::URL url_;
  std::unique_ptr<proxygen::HTTPConnector> conn_;
  proxygen::HTTPUpstreamSession* sess_;
  // std::atomic<bool> ready_{false};
  folly::EventBase evb_;
};

class CliHandle : public proxygen::HTTPTransactionHandler {
 public:
  explicit CliHandle(proxygen::HTTPUpstreamSession* sess);
  ~CliHandle();

  // HTTPTransactionHandler methods
  void setTransaction(proxygen::HTTPTransaction* txn) noexcept override;
  void detachTransaction() noexcept override;
  void onHeadersComplete(
      std::unique_ptr<proxygen::HTTPMessage> msg) noexcept override;
  void onBody(std::unique_ptr<folly::IOBuf> chain) noexcept override;
  void onTrailers(
      std::unique_ptr<proxygen::HTTPHeaders> trailers) noexcept override;
  void onEOM() noexcept override;
  void onUpgrade(proxygen::UpgradeProtocol protocol) noexcept override;
  void onError(const proxygen::HTTPException& error) noexcept override;
  void onEgressPaused() noexcept override;
  void onEgressResumed() noexcept override;

  // self get
  void sendMsg(const proxygen::HTTPMethod& httpMethod, const proxygen::URL& url,
               const proxygen::HTTPHeaders& headers,
               const std::string&& inputStr);
  std::unique_ptr<proxygen::HTTPMessage> getRspHeader();
  std::unique_ptr<std::string> getRspBody();

 private:
  proxygen::HTTPUpstreamSession* sess_;
  proxygen::HTTPTransaction* txn_;
  std::unique_ptr<proxygen::HTTPMessage> response_;
  std::unique_ptr<std::string> outputStr_;
  bool egressPaused_{false};
  std::string inputStr_;
  // folly::fibers::Baton baton_;
  // folly::Promise<int> hasRspP_;
  // folly::Future<int> hasRsp_;
};

class CliManage {
 public:
  explicit CliManage(const std::string& addrUrl, size_t num);
  ~CliManage();

  void send(const std::string& method, const std::string& url,
            const std::string& head, const std::string& req, std::string& rsp,
            folly::F14FastMap<std::string, std::string>& rspHead);

 private:
  std::string addr_;
  folly::fbvector<CliSession> sesses_;
};

class CliManageFactory {
 public:
  explicit CliManageFactory();
  ~CliManageFactory() = default;
  static CliManageFactory& singleton();

  enum class AddrOp { Add, Del };

  bool addCliManage(const std::string& name, const std::string& addrUrl,
                    size_t num);
  bool cliChgAddr(AddrOp op, const std::string& name, const std::string& addr);
  folly::HHWheelTimer* getTimer();
  CliManage* getClient(const std::string& name);

 private:
  folly::EventBase evb_;
  folly::HHWheelTimer::UniquePtr timer_;
  folly::F14FastMap<folly::fbstring,
                    folly::fbvector<std::unique_ptr<CliManage>>>
      cliMap_;
};

}  // namespace HttpClient