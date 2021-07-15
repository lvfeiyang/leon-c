#include "HttpClient.h"

#include <proxygen/lib/http/HTTPCommonHeaders.h>
// #include <proxygen/lib/http/codec/HTTP2Codec.h>

namespace HttpClient {

CliSession::CliSession(const std::string& addrurl) : evb_() {
  url_ = std::move(proxygen::URL(addrurl));
  conn_ = std::make_unique<proxygen::HTTPConnector>(
      this, CliManageFactory::singleton().getTimer());

  folly::SocketAddress addr(url_.getHost(), url_.getPort());
  // LOG(INFO) << "leon_debug addr " << addrurl << " " << addr.describe();
  conn_->connect(&evb_, addr, std::chrono::milliseconds(1000), opts);
  evb_.loop();  // Forever();
}
CliSession::CliSession(CliSession&& clisess) {
  url_ = std::move(clisess.url_);
  conn_ = std::move(clisess.conn_);
  sess_ = clisess.sess_;
  // ready_ = clisess.ready_.load();
}
void CliSession::connectSuccess(proxygen::HTTPUpstreamSession* session) {
  // LOG(INFO) << "leon_debug conn succ";
  // VLOG(5) << "vlog test";
  // DLOG(INFO) << "dlog test";
  session->setFlowControl(65536, 65536, 65536);
  sess_ = session;
}
void CliSession::connectError(const folly::AsyncSocketException& ex) {
  LOG(ERROR) << "Coudln't connect to " << url_.getHostAndPort() << ":"
             << ex.what();
}

folly::Optional<proxygen::HTTPUpstreamSession*> CliSession::getSession() {
  // or future wait ready
  return sess_;
}

CliHandle::CliHandle(proxygen::HTTPUpstreamSession* sess) : sess_(sess) {
  txn_ = sess_->newTransaction(this);
}
CliHandle::~CliHandle() {}
void CliHandle::setTransaction(proxygen::HTTPTransaction* txn) noexcept {
  txn_ = txn;
}
void CliHandle::detachTransaction() noexcept {
  VLOG(5) << "leon_debug detach txn";
}
void CliHandle::onHeadersComplete(
    std::unique_ptr<proxygen::HTTPMessage> msg) noexcept {
  VLOG(5) << "leon_debug onHeadersComplete";
  response_ = std::move(msg);
}
void CliHandle::onBody(std::unique_ptr<folly::IOBuf> chain) noexcept {
  VLOG(5) << "leon_debug onBody";
  if (chain) {
    outputStr_ = std::make_unique<std::string>();
    const folly::IOBuf* p = chain.get();
    do {
      outputStr_->append((const char*)p->data(), p->length());
      p = p->next();
    } while (p != chain.get());
  }
  return;
}
void CliHandle::onTrailers(
    std::unique_ptr<proxygen::HTTPHeaders> trailers) noexcept {
  VLOG(5) << "leon_debug onTrailers";
}
void CliHandle::onEOM() noexcept { VLOG(5) << "leon_debug onEOM"; }
void CliHandle::onUpgrade(proxygen::UpgradeProtocol protocol) noexcept {
  VLOG(5) << "Discarding upgrade protocol";
}
void CliHandle::onError(const proxygen::HTTPException& error) noexcept {
  LOG(ERROR) << "An error occurred: " << error.what();
}
void CliHandle::onEgressPaused() noexcept {
  VLOG(5) << "Egress paused";
  egressPaused_ = true;
}
void CliHandle::onEgressResumed() noexcept {
  VLOG(5) << "Egress resumed";
  egressPaused_ = false;
  if (inputStr_.size() != 0) {
    std::unique_ptr<folly::IOBuf> buf(
        folly::IOBuf::wrapBuffer(inputStr_.data(), inputStr_.size()));
    txn_->sendBody(std::move(buf));
    txn_->sendEOM();
  }
}

void CliHandle::sendMsg(const proxygen::HTTPMethod& httpMethod,
                        const proxygen::URL& url,
                        const proxygen::HTTPHeaders& headers,
                        const std::string&& inputStr) {
  proxygen::HTTPMessage request;
  headers.forEach(
      [&request](const std::string& header, const std::string& val) {
        request.getHeaders().add(header, val);
      });
  request.setMethod(httpMethod);
  request.setHTTPVersion(1, 1);
  request.setURL(url.makeRelativeURL());
  if (!request.getHeaders().getNumberOfValues(
          proxygen::HTTP_HEADER_USER_AGENT)) {
    request.getHeaders().add(proxygen::HTTP_HEADER_USER_AGENT, "proxygen_curl");
  }
  if (!request.getHeaders().getNumberOfValues(proxygen::HTTP_HEADER_HOST)) {
    request.getHeaders().add(proxygen::HTTP_HEADER_HOST, url.getHostAndPort());
  }
  if (!request.getHeaders().getNumberOfValues(proxygen::HTTP_HEADER_ACCEPT)) {
    request.getHeaders().add("Accept", "*/*");
  }
  request.dumpMessage(5);

  txn_->sendHeaders(request);
  // DLOG(INFO) << "leon_debug send header";
  if (proxygen::HTTPMethod::POST == httpMethod ||
      proxygen::HTTPMethod::PUT == httpMethod) {
    inputStr_ = inputStr;
    if (inputStr_.size() > 0) {
      if (!egressPaused_) {
        std::unique_ptr<folly::IOBuf> buf(
            folly::IOBuf::wrapBuffer(inputStr_.data(), inputStr_.size()));
        txn_->sendBody(std::move(buf));
        // txn_->sendEOM();
        // DLOG(INFO) << "leon_debug send body";
      }
    }
  }
  txn_->sendEOM();

  // sess_->closeWhenIdle(); // EVLOOP_NONBLOCK
  sess_->getEventBase()->loopOnce();  // for send
  sess_->getEventBase()->loopOnce();  // for receive
}
std::unique_ptr<proxygen::HTTPMessage> CliHandle::getRspHeader() {
  return std::move(response_);
}
std::unique_ptr<std::string> CliHandle::getRspBody() {
  return std::move(outputStr_);
}

proxygen::HTTPHeaders parseHeaders(const std::string& headersString) {
  std::vector<folly::StringPiece> headersList;
  proxygen::HTTPHeaders headers;
  folly::split(",", headersString, headersList);
  for (const auto& headerPair : headersList) {
    std::vector<folly::StringPiece> nv;
    folly::split('=', headerPair, nv);
    if (nv.size() > 0) {
      if (nv[0].empty()) {
        continue;
      }
      std::string value("");
      for (size_t i = 1; i < nv.size(); i++) {
        value += folly::to<std::string>(nv[i], '=');
      }
      if (nv.size() > 1) {
        value.pop_back();
      }  // trim anything else
      headers.add(nv[0], value);
    }
  }
  return headers;
}

CliManage::CliManage(const std::string& addrUrl, size_t num) {
  // sesses_.reserve(num);
  for (size_t i = 0; i < num; i++) {
    // sesses_[i] = std::move(CliSession(addrUrl));
    sesses_.emplace_back(addrUrl);
  }
  addr_ = addrUrl;
}
CliManage::~CliManage() {}
void CliManage::send(const std::string& method, const std::string& url,
                     const std::string& head, const std::string& req,
                     std::string& rsp,
                     folly::F14FastMap<std::string, std::string>& rspHead) {
  // get one txn
  CliHandle handle(sesses_[0].getSession().value());
  // folly::FiberIOExecutor ioe(httpCliTpe);
  // ioe.add([&]() {
  // txn handle for send
  handle.sendMsg(proxygen::stringToMethod(method).value(),
                 proxygen::URL(addr_ + url), parseHeaders(head),
                 std::move(req));
  // });
  // if (!handle.waitRsp(std::chrono::milliseconds(2000))) {
  //   handle.detachTransaction();
  //   LOG(ERROR) << "get rsp timeout";
  //   return;
  // }
  // return;
  auto rspp = handle.getRspBody();
  if (rspp) {
    rsp = *rspp;
  }
  handle.getRspHeader()->getHeaders().forEach(
      [&rspHead](const std::string& header, const std::string& val) {
        rspHead.insert(std::pair<std::string, std::string>(header, val));
      });
}

CliManageFactory::CliManageFactory() : evb_(), cliMap_(16) {
  timer_ = std::move(folly::HHWheelTimer::newTimer(
      &evb_,
      std::chrono::milliseconds(folly::HHWheelTimer::DEFAULT_TICK_INTERVAL),
      folly::AsyncTimeout::InternalEnum::NORMAL,
      std::chrono::milliseconds(5000)));
}
// CliManageFactory::~CliManageFactory() {}
CliManageFactory& CliManageFactory::singleton() {
  static CliManageFactory cmfac;
  return cmfac;
}
folly::HHWheelTimer* CliManageFactory::getTimer() { return timer_.get(); }
bool CliManageFactory::addCliManage(const std::string& name,
                                    const std::string& addrUrl, size_t num) {
  if (this->cliMap_.find(name) == this->cliMap_.end()) {
    this->cliMap_.insert(
        std::make_pair<folly::fbstring,
                       folly::fbvector<std::unique_ptr<CliManage>>>(
            name, folly::fbvector<std::unique_ptr<CliManage>>()));
  }
  this->cliMap_.at(name).emplace_back(
      std::make_unique<CliManage>(addrUrl, num));
}
bool CliManageFactory::cliChgAddr(AddrOp op, const std::string& name,
                                  const std::string& addr) {}
CliManage* CliManageFactory::getClient(const std::string& name) {
  if (this->cliMap_.find(name) != this->cliMap_.end()) {
    auto& cliv = this->cliMap_.at(name);
    return cliv.at(std::rand() % cliv.size()).get();
  }
  return nullptr;
}

}  // namespace HttpClient