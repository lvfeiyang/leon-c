#include "Service.h"

#include <proxygen/httpserver/ResponseBuilder.h>

#include "Msg.h"
#include "cache/Cache.h"
#include "db/RedisDB.h"

namespace FsService {

void FsHandlerFactory::onServerStart(folly::EventBase* /*evb*/) noexcept {}

void FsHandlerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* FsHandlerFactory::onRequest(
    proxygen::RequestHandler*, proxygen::HTTPMessage* head) noexcept {
  if (head->getURL() == "/pull") {
    return new FsHandler(InOutMsg::MsgType::Pull);
  } else if (head->getURL() == "/push") {
    return new FsHandler(InOutMsg::MsgType::Push);
  } else if (head->getURL() == "/delcache") {
    return new FsHandler(InOutMsg::MsgType::DelCache);
  } else {
    return new FsHandler(InOutMsg::MsgType::Unknow);
  }
}

// write handle
FsHandler::FsHandler(InOutMsg::MsgType t) : reqType_(t) {}

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
  InOutMsg msg(body->length(), body->data(), this->reqType_);

  if (this->reqType_ == InOutMsg::MsgType::Unknow) {
    proxygen::ResponseBuilder(downstream_)
        .body(msg.EncodeRsp(1))
        .send();  // unknow url
    return;
  }

  switch (this->reqType_) {
    case InOutMsg::MsgType::Pull: {
      folly::fbvector<uint64_t> keys = msg.getPullIds();
      // folly::fbvector<folly::fbstring> datas;
      int64_t _currv;
      uint64_t k;
      folly::fbstring d;
      // for (auto k : keys) {
      for (size_t i = 0, n = keys.size(); i < n; i++) {
        k = keys[i];
        d = Cache::CacheSpace::singleton().get(k);
        if (d.empty()) {
          d = RedisDB::G_DBInsts[rand() % RedisDB::DBInstNum].get(k, _currv);
          Cache::CacheSpace::singleton().set(k, d);
        } else {
          LOG(INFO) << "leon_debug get from cache";
        }
        msg.setPullData(i, d);
      }
      LOG(INFO) << "leon_debug: " << msg.show();

      proxygen::ResponseBuilder(downstream_).body(msg.EncodeRsp(0)).send();
      break;
    }
    case InOutMsg::MsgType::Push: {
      InOutMsg::DataInfo kv;
      int64_t _currv = 0;
      folly::fbstring d;
      folly::fbvector<InOutMsg::DataInfo>& infos = msg.getInfos();
      RedisDB::DBInst dbinst = RedisDB::G_DBInsts[rand() % RedisDB::DBInstNum];
      for (size_t i = 0, n = infos.size(); i < n; i++) {
        kv = infos[i];
        // cas
        d = dbinst.get(kv.id, _currv);
        d += kv.d;  // merge(d, kv.d);
        auto setRet = dbinst.set(kv.id, d, _currv);
        LOG(INFO) << "leon_debug: " << setRet;
      }
      proxygen::ResponseBuilder(downstream_).body(msg.EncodeRsp(0)).send();
    }
    case InOutMsg::MsgType::DelCache: {
      folly::fbvector<uint64_t> keys = msg.getPullIds();
      for (auto k : keys) {
        Cache::CacheSpace::singleton().del(k);
      }
      proxygen::ResponseBuilder(downstream_).body(msg.EncodeRsp(0)).send();
    }

    default:
      break;
  }

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