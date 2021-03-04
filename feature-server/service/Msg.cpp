#include "Msg.h"

#include <folly/Conv.h>
#include <glog/logging.h>

#include "util/pbdata/serv-msg.pb.h"

namespace FsService {
// decode msg
/*
PushMsg::PushMsg(size_t len, const uint8_t* d) {
  while (len >= sizeof(uint64_t))  // 还存在id
  {
    uint8_t oneOffset = sizeof(uint64_t);
    uint64_t pushId = folly::Endian::big(*(uint64_t*)d);
    len -= oneOffset;
    d += oneOffset;

    if (len < sizeof(uint32_t)) {
      this->infos.push_back(PushMsg::
                            writeInfo{id : pushId, wd : folly::fbstring()});
      continue;
    }
    oneOffset = sizeof(uint32_t);
    uint32_t pbLen = folly::Endian::big(*(uint32_t*)d);
    len -= oneOffset;
    d += oneOffset;

    folly::fbstring wd((char*)d, pbLen);
    len -= pbLen;
    d += pbLen;

    this->infos.push_back(PushMsg::writeInfo{id : pushId, wd : wd});
  }
  return;
}
PushMsg::~PushMsg() {}
folly::fbstring PushMsg::show() {
  return folly::to<folly::fbstring>(this->infos[0].id);
}
*/

InOutMsg::InOutMsg(size_t len, const uint8_t* d, MsgType t) {
  this->t_ = t;
  switch (t) {
    case MsgType::Push: {
      ServMsg::PushReq req;
      req.ParseFromString(std::string((char*)d, len));
      // req.ParseFromIstream(d);
      // LOG(INFO) << "leon_debug " << req.DebugString();
      size_t num = req.infos_size();
      this->infos_.reserve(num);
      // ServMsg::oneInfo onei;
      for (size_t i = 0; i < num; i++) {
        auto onei = req.infos(i);
        this->infos_.push_back(DataInfo{id : onei.key(), d : onei.data()});
      }
      break;
    }
    case MsgType::Pull: {
      ServMsg::PullReq req;
      req.ParseFromString(std::string((char*)d, len));
      size_t num = req.keys_size();
      this->infos_.reserve(num);
      for (size_t i = 0; i < num; i++) {
        this->infos_.push_back(DataInfo{id : req.keys(i)});
      }
      num = req.field_size();
      for (size_t i = 0; i < num; i++) {
        this->fields_.insert(req.field(i));
      }
      break;
    }
    default:
      break;
  }
}
InOutMsg::~InOutMsg() {}
folly::fbstring InOutMsg::show() {
  folly::fbstring ret;
  for (auto i : this->infos_) {
    ret += folly::to<folly::fbstring>(i.id) + ":" + i.d + ",";
  }
  ret += '\n';
  for (auto i : this->fields_) {
    ret += folly::to<folly::fbstring>(i) + ",";
  }
  return ret;
}
folly::fbvector<uint64_t> InOutMsg::getPullIds() {
  // folly::fbvector<uint64_t> ret(this->infos_.size());
  // for (size_t i = 0, n = this->infos_.size(); i < n; i++) {
  //   ret[i] = this->infos_[i].id;
  // }
  // return ret;
  folly::fbvector<uint64_t> ret;
  ret.reserve(this->infos_.size());
  for (auto v : this->infos_) {
    ret.push_back(v.id);
  }
  return ret;
}
void InOutMsg::setPullData(size_t i, folly::fbstring d) {
  this->infos_[i].d = d;
}
std::string InOutMsg::EncodeRsp(int32_t errcode) {
  std::string ret;
  switch (this->t_) {
    case MsgType::Pull: {
      ServMsg::PullRsp rsp;
      if (0 != errcode) {
        rsp.set_errcode(errcode);
      } else {
        for (auto v : this->infos_) {
          rsp.add_data(folly::toStdString(v.d));
        }
      }
      rsp.SerializeToString(&ret);
      break;
    }

    default:
      break;
  }
  return ret;
}

}  // namespace FsService