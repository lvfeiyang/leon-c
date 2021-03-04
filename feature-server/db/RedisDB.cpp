#include "RedisDB.h"

#include <folly/Conv.h>
#include <glog/logging.h>

namespace RedisDB {
DBInst::DBInst() {
  struct timeval timeout = {1, 500000};
  this->dbCon = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
  redisReply* reply = (redisReply*)redisCommand(this->dbCon, "AUTH %s", "test");
  freeReplyObject(reply);
}
DBInst::~DBInst() { redisFree(this->dbCon); }
folly::fbstring DBInst::get(uint64_t k, int64_t& currv) {
  folly::fbstring ret;
  redisReply* reply = (redisReply*)redisCommand(this->dbCon, "HGETALL %d", k);
  DLOG(INFO) << "redis reply type " << reply->type;
  switch (reply->type) {
    case REDIS_REPLY_ERROR:
      LOG(ERROR) << "redis get error: " << reply->str;
      break;
    case REDIS_REPLY_ARRAY:
      if (reply->elements > 1 && reply->elements % 2 == 0) {
        // ret = folly::fbstring(reply->element[1]->str,
        // reply->element[1]->len);
        int64_t maxv = 0;
        size_t maxi = 0;
        for (size_t i = 0; i < reply->elements; i += 2) {
          int64_t tmpv = folly::to<int64_t>(reply->element[i]->str);
          if (tmpv > maxv) {
            maxv = tmpv;
            maxi = i;
          }
        }
        currv = maxv;
        ret = reply->element[++maxi]->str;
      }
      break;

    default:
      break;
  }
  // folly::fbstring ret(reply->str, reply->len);
  freeReplyObject(reply);
  return ret;
}
bool DBInst::set(uint64_t k, folly::fbstring v, int64_t currv) {
  bool ret = false;
  redisReply* reply =
      (redisReply*)redisCommand(this->dbCon, "HSETNX %d %d %s", k, currv, v);
  switch (reply->type) {
    case REDIS_REPLY_ERROR:
      LOG(ERROR) << "redis set error: " << reply->str;
      break;
    case REDIS_REPLY_INTEGER:
      if (reply->integer == 1) ret = true;
      break;

    default:
      break;
  }
  freeReplyObject(reply);
  return ret;
}

folly::fbvector<DBInst> G_DBInsts = folly::fbvector<DBInst>(DBInstNum);

}  // namespace RedisDB