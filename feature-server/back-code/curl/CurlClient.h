/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <folly/io/async/EventBase.h>
#include <folly/io/async/SSLContext.h>
#include <proxygen/lib/http/HTTPConnector.h>
#include <proxygen/lib/http/session/HTTPTransaction.h>
#include <proxygen/lib/utils/URL.h>

#include <fstream>

namespace CurlService {

class CurlClient : public proxygen::HTTPConnector::Callback,
                   public proxygen::HTTPTransactionHandler {
  class CurlPushHandler : public proxygen::HTTPTransactionHandler {
   public:
    explicit CurlPushHandler(CurlClient* parent) : parent_{parent} {}

    void setTransaction(proxygen::HTTPTransaction* /*txn*/) noexcept override;

    void detachTransaction() noexcept override;

    void onHeadersComplete(
        std::unique_ptr<proxygen::HTTPMessage> /*msg*/) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> /*chain*/) noexcept override;

    void onEOM() noexcept override;

    void onError(const proxygen::HTTPException& /*error*/) noexcept override;

    void onTrailers(
        std::unique_ptr<proxygen::HTTPHeaders> /*trailers*/) noexcept override {
    }

    void onUpgrade(proxygen::UpgradeProtocol /*protocol*/) noexcept override {}

    void onEgressResumed() noexcept override {}

    void onEgressPaused() noexcept override {}

   private:
    // hack around the ambiguous API
    bool seenOnHeadersComplete_{false};
    // the pushed transaction
    proxygen::HTTPTransaction* pushedTxn_{nullptr};
    // information about the request

    std::unique_ptr<proxygen::HTTPMessage> promise_;
    std::unique_ptr<proxygen::HTTPMessage> response_;
    CurlClient* parent_;
  };

 public:
  CurlClient(const std::string& urlPrefix, const proxygen::URL& url,
             bool h2c = false, unsigned short httpMajor = 1,
             unsigned short httpMinor = 1, bool partiallyReliable = false);

  virtual ~CurlClient() = default;

  void setSendMsg(std::string&& req);
  void setRespMsg(std::string* rsp);
  void setHeader(const proxygen::HTTPHeaders& headers);
  void setMethod(proxygen::HTTPMethod httpMethod);
  void setUrl(const std::string& url);

  static proxygen::HTTPHeaders parseHeaders(const std::string& headersString);

  // initial SSL related structures
  void initializeSsl(const std::string& caPath, const std::string& nextProtos,
                     const std::string& certPath = "",
                     const std::string& keyPath = "");
  void sslHandshakeFollowup(proxygen::HTTPUpstreamSession* session) noexcept;

  // HTTPConnector methods
  void connectSuccess(proxygen::HTTPUpstreamSession* session) override;
  void connectError(const folly::AsyncSocketException& ex) override;

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
  void onPushedTransaction(
      proxygen::HTTPTransaction* /* pushedTxn */) noexcept override;

  void sendRequest(proxygen::HTTPTransaction* txn);

  // Getters
  folly::SSLContextPtr getSSLContext() { return sslContext_; }

  const std::string& getServerName() const;

  void setFlowControlSettings(int32_t recvWindow);

  const proxygen::HTTPMessage* getResponse() const { return response_.get(); }

  void setLogging(bool enabled) { loggingEnabled_ = enabled; }

  void setEOMFunc(std::function<void()> eomFunc) { eomFunc_ = eomFunc; }

  bool hasConnect() { return txn_ != nullptr; }

 protected:
  void sendBodyFromFile();

  void setupHeaders();

  void printMessageImpl(proxygen::HTTPMessage* msg,
                        const std::string& tag = "");

  proxygen::HTTPTransaction* txn_{nullptr};
  proxygen::HTTPUpstreamSession* session_{nullptr};
  proxygen::HTTPMethod httpMethod_;
  std::string urlPrefix_;
  proxygen::URL url_;
  proxygen::HTTPMessage request_;
  folly::SSLContextPtr sslContext_;
  int32_t recvWindow_{0};
  bool loggingEnabled_{false};
  bool h2c_{false};
  unsigned short httpMajor_;
  unsigned short httpMinor_;
  bool egressPaused_{false};
  bool partiallyReliable_{false};

  std::string inputStr_;
  std::string* outputStr_;

  std::unique_ptr<proxygen::HTTPMessage> response_;
  std::vector<std::unique_ptr<CurlPushHandler>> pushTxnHandlers_;

  folly::Optional<std::function<void()>> eomFunc_;

  friend class CurlPushHandler;
};

}  // namespace CurlService
