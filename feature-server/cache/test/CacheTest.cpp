#include <gtest/gtest.h>

#include "cache/Cache.h"

TEST(module_cache, test_sgd) {
  auto ret = Cache::CacheSpace::singleton().set(11, "test");
  auto getret = Cache::CacheSpace::singleton().get(11);
  Cache::CacheSpace::singleton().del(11);
  auto getret1 = Cache::CacheSpace::singleton().get(11);
  std::cout << ret << ", " << getret << ", " << getret1 << std::endl;
}