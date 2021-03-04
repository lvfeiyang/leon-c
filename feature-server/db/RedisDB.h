#pragma once

#include <folly/FBString.h>
#include <folly/FBVector.h>
#include <hiredis/hiredis.h>

namespace RedisDB {
class DBInst {
 public:
  explicit DBInst();
  ~DBInst();

  bool set(uint64_t k, folly::fbstring v, int64_t currv);
  folly::fbstring get(uint64_t k, int64_t& currv);

 private:
  redisContext* dbCon;
};
// threadPool add func, func
// chose context do redis get

const size_t DBInstNum = 10;
extern folly::fbvector<DBInst> G_DBInsts;

}  // namespace RedisDB