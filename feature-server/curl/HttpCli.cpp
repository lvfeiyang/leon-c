#include "curl/HttpCli.h"

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
void CliManage::send(std::string method, std::string url, std::string head,
                     std::string req, std::string* rsp) {
  size_t index = std::rand() % cliNum_;

  this->cliConns_[index]->client->setSendMsg(std::move(req));
  this->cliConns_[index]->client->setRespMsg(rsp);
  proxygen::HTTPHeaders headers = CurlService::CurlClient::parseHeaders(head);
  this->cliConns_[index]->client->setHeader(headers);
  proxygen::HTTPMethod httpMethod = *proxygen::stringToMethod(method);
  this->cliConns_[index]->client->setMethod(httpMethod);
  if (url.size() > 0) this->cliConns_[index]->client->setUrl(url);

  this->cliConns_[index]->connector->connect(
      &this->cliConns_[index]->evb, *this->addr_,
      std::chrono::milliseconds(1000), opts);
  this->cliConns_[index]->evb.loop();
}
}  // namespace HttpCli