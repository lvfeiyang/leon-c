#pragma once

#include <folly/FBString.h>
#include <folly/container/EvictingCacheMap.h>

namespace Cache {
class CacheSpace {
 public:
  explicit CacheSpace();
  ~CacheSpace();
  static CacheSpace& singleton();
  bool set(uint64_t k, folly::fbstring v);
  folly::fbstring get(uint64_t k);
  void del(uint64_t k);

 private:
  folly::EvictingCacheMap<uint64_t, folly::fbstring> cacheMap_;
};
}  // namespace Cache