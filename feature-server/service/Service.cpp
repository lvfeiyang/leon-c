#include "Service.h"

#include <proxygen/httpserver/ResponseBuilder.h>

#include "Msg.h"
#include "db/RedisDB.h"

namespace FsService {

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
  InOutMsg msg(body->length(), body->data(), InOutMsg::MsgType::Pull);

  folly::fbvector<uint64_t> keys = msg.getPullIds();
  // folly::fbvector<folly::fbstring> datas;
  int64_t _currv;
  uint64_t k;
  folly::fbstring d;
  // for (auto k : keys) {
  for (size_t i = 0, n = keys.size(); i < n; i++) {
    k = keys[i];
    // cache.get();

    // redis.get();
    d = RedisDB::G_DBInsts[rand() % RedisDB::DBInstNum].get(k, _currv);
    msg.setPullData(i, d);

    // cache.set();
  }
  LOG(INFO) << "leon_debug: " << msg.show();

  proxygen::ResponseBuilder(downstream_).body(msg.EncodeRsp(0)).send();

  // proxygen::ResponseBuilder(downstream_).body(std::move(body)).send();
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

}  // namespace FsService