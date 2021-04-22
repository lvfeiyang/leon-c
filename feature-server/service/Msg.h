#pragma once

#include <folly/FBString.h>
#include <folly/FBVector.h>
#include <folly/container/F14Set.h>
#include <stdint.h>

namespace FsService {
// void decodeMsg(size_t l, const uint8_t* d);

class InOutMsg {
 public:
  enum class MsgType { Push, Pull, DelCache, Unknow };
  struct DataInfo {
    uint64_t id;
    folly::fbstring d;
  };
  explicit InOutMsg(size_t len, const uint8_t* d, MsgType t);
  ~InOutMsg();

  folly::fbstring show();
  folly::fbvector<uint64_t> getPullIds();
  folly::fbvector<DataInfo>& getInfos();
  void setPullData(size_t i, folly::fbstring d);
  std::string EncodeRsp(int32_t errcode);

 private:
  folly::fbvector<DataInfo> infos_;
  folly::F14FastSet<uint64_t> fields_;
  MsgType t_;
};

}  // namespace FsService