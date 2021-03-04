#include <gtest/gtest.h>

#include "util/Yaml.h"

TEST(module_util, test_yaml) {
  Util::YamlConf conf("config.yaml");
  std::string a = conf.get<std::string>("name");
  std::cout << a << std::endl;
}