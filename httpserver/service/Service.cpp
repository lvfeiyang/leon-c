#include "Service.h"

#include <proxygen/httpserver/ResponseBuilder.h>

namespace HService {

void FsHandlerFactory::onServerStart(folly::EventBase* /*evb*/) noexcept {}

void FsHandlerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* FsHandlerFactory::onRequest(
    proxygen::RequestHandler*, proxygen::HTTPMessage* head) noexcept {
  return new FsHandler();
}

// write handle
void FsHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> req) noexcept {
  proxygen::ResponseBuilder builder(downstream_);
  builder.status(200, "OK");
  req->getHeaders().forEach([&](std::string& name, std::string& value) {
    builder.header(folly::to<std::string>("x-echo-", name), value);
  });
  // LOG(INFO) << req->getMethodString() << req->getURL() << req->getPath()
  //           << req->getQueryString();
  builder.send();
}

void FsHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
  // proxygen::ResponseBuilder(downstream_).body("hello\n").send();
}

// void decodeMsg(size_t len, const uint8_t* d) {
//   auto c = folly::Endian::big(*(uint64_t*)d);
//   d = d + 2;
//   LOG(INFO) << len << "," << d << "," << std::hex << c;
// }

void FsHandler::onEOM() noexcept {
  proxygen::ResponseBuilder(downstream_).sendWithEOM();
}

void FsHandler::onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept {
  // handler doesn't support upgrades
}

void FsHandler::requestComplete() noexcept { delete this; }

void FsHandler::onError(proxygen::ProxygenError /*err*/) noexcept {
  delete this;
}

}  // namespace HService