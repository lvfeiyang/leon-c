#include "Yaml.h"

namespace Util {
YamlConf::YamlConf(const std::string& path) { this->yn = YAML::LoadFile(path); }
YamlConf::~YamlConf() {}

YamlConf G_SerConf("config.yaml");
}  // namespace Util