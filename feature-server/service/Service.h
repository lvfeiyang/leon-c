#pragma once

#include <proxygen/httpserver/RequestHandlerFactory.h>

#include "Msg.h"

namespace FsService {

class FsHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* evb) noexcept override;

  void onServerStop() noexcept override;

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler*, proxygen::HTTPMessage* head) noexcept override;
};

class FsHandler : public proxygen::RequestHandler {
 public:
  explicit FsHandler(InOutMsg::MsgType t);

  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

  void onEOM() noexcept override;

  void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

  void requestComplete() noexcept override;

  void onError(proxygen::ProxygenError err) noexcept override;

 private:
  InOutMsg::MsgType reqType_;
};

}  // namespace FsService
