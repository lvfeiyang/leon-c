#pragma once

#include <yaml-cpp/yaml.h>

namespace Util {
class YamlConf {
 public:
  explicit YamlConf(const std::string& path);
  ~YamlConf();

  template <typename T>
  T get(const std::string& k) {
    return this->yn[k].as<T>();
  }

 private:
  YAML::Node yn;
};
}  // namespace Util
