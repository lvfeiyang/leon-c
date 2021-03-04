#include <gtest/gtest.h>

#include "db/RedisDB.h"

TEST(module_db, test_get) {
  RedisDB::DBInst onedbinst;
  int64_t currv;
  std::cout << onedbinst.get(11, currv) << ", " << currv << std::endl;
  std::cout << onedbinst.set(11, "test_4", 4) << std::endl;

  auto a = RedisDB::G_DBInsts[rand() % RedisDB::DBInstNum].get(11, currv);
  std::cout << a << std::endl;
}
