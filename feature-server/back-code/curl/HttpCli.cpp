#include "curl/HttpCli.h"

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/GlobalExecutor.h>
#include <folly/executors/IOThreadPoolExecutor.h>

namespace HttpCli {
CliManageFactory::CliManageFactory() : evb_(), cliMap_(16) {
  timer_ = std::move(folly::HHWheelTimer::newTimer(
      &evb_,
      std::chrono::milliseconds(folly::HHWheelTimer::DEFAULT_TICK_INTERVAL),
      folly::AsyncTimeout::InternalEnum::NORMAL,
      std::chrono::milliseconds(5000)));
}
CliManageFactory::~CliManageFactory() {}
CliManageFactory& CliManageFactory::singleton() {
  static CliManageFactory cmfac;
  return cmfac;
}
folly::HHWheelTimer* CliManageFactory::getTimer() { return timer_.get(); }
bool CliManageFactory::addCliManage(std::string name, std::string addrUrl,
                                    size_t num) {
  if (this->cliMap_.find(name) == this->cliMap_.end()) {
    this->cliMap_.insert(
        std::make_pair<folly::fbstring,
                       folly::fbvector<std::unique_ptr<CliManage>>>(
            name, folly::fbvector<std::unique_ptr<CliManage>>()));
  }
  this->cliMap_.at(name).emplace_back(
      std::make_unique<CliManage>(addrUrl, num));
}
bool CliManageFactory::cliChgAddr(AddrOp op, std::string name,
                                  std::string addr) {}
CliManage* CliManageFactory::getClient(std::string name) {
  if (this->cliMap_.find(name) != this->cliMap_.end()) {
    auto& cliv = this->cliMap_.at(name);
    return cliv.at(std::rand() % cliv.size()).get();
  }
  return nullptr;
}

CliManage::CliManage(std::string addrUrl, size_t num) {
  proxygen::URL url(addrUrl);
  addr_ = std::make_unique<folly::SocketAddress>(url.getHost(), url.getPort(),
                                                 true);
  cliConns_.resize(num);
  for (size_t i = 0; i < num; i++) {
    cliConns_.at(i) = std::make_unique<connResource>();
    cliConns_[i]->client =
        std::make_unique<CurlService::CurlClient>(addrUrl, url);
    cliConns_[i]->client->setFlowControlSettings(65536);
    // cliConns_[i].client ->setLogging(true);
    cliConns_[i]->connector = std::make_unique<proxygen::HTTPConnector>(
        cliConns_[i]->client.get(), CliManageFactory::singleton().getTimer());
  }
  cliNum_ = num;
}
CliManage::~CliManage() {}
std::thread testThread;
folly::IOThreadPoolExecutor ioExe(1);
void CliManage::send(std::string method, std::string url, std::string head,
                     std::string req, std::string* rsp,
                     folly::F14FastMap<std::string, std::string>& rspHead) {
  size_t index = std::rand() % cliNum_;

  auto* oneIndex = this->cliConns_[index].get();
  oneIndex->client->setSendMsg(std::move(req));
  oneIndex->client->setRespMsg(rsp);
  proxygen::HTTPHeaders headers = CurlService::CurlClient::parseHeaders(head);
  oneIndex->client->setHeader(headers);
  proxygen::HTTPMethod httpMethod = *proxygen::stringToMethod(method);
  oneIndex->client->setMethod(httpMethod);
  if (url.size() > 0) oneIndex->client->setUrl(url);

  if (oneIndex->client->hasConnect()) {
		CurlService::CurlClient::CurlPushHandler pushHandle(oneIndex->client.get());
    proxygen::HTTPTransaction* pushedTxn = oneIndex->client->newExTransaction(&pushHandle);
    // oneIndex->client->onPushedTransaction(pushedTxn);
    oneIndex->client->sendRequest(pushedTxn);
  } else {
    // folly::ThreadedExecutor x; folly::getGlobalIOExecutor()
    // folly::CPUThreadPoolExecutor ioExe(1);
    ioExe.add([&]() {
      // testThread = std::move(std::thread([&]() {
      oneIndex->connector->connect(&oneIndex->evb, *this->addr_,
                                   std::chrono::milliseconds(1000), opts);
      LOG(INFO) << "leon_debug run in thread";
      oneIndex->evb.loop();
    });
  }
  LOG(INFO) << "leon_debug run thread";
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  oneIndex->client->getResponse()->getHeaders().forEach(
      [&rspHead](const std::string& header, const std::string& val) {
        rspHead.insert(std::pair<std::string, std::string>(header, val));
      });
}
}  // namespace HttpCli