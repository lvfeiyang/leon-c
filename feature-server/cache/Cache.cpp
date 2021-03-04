#include "Cache.h"

#include <glog/logging.h>

namespace Cache {
CacheSpace::CacheSpace() : cacheMap_(100, 10) {}
CacheSpace::~CacheSpace() {}
CacheSpace& CacheSpace::singleton() {
  static CacheSpace c;
  return c;
}
bool CacheSpace::set(uint64_t k, folly::fbstring v) {
  // LOG(INFO) << "leon_debug " << this->cacheMap_.getMaxSize();
  this->cacheMap_.set(k, v);
  return true;
}
folly::fbstring CacheSpace::get(uint64_t k) {
  folly::fbstring ret;
  auto it = this->cacheMap_.find(k);
  if (it != this->cacheMap_.end()) {
    ret = it->second;
  }
  return ret;
  // return this->cacheMap_.get(k);
}
void CacheSpace::del(uint64_t k) { this->cacheMap_.erase(k); }
}  // namespace Cache