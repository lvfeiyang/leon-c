cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/home/lxj/proxygen/proxygen/_build/deps/include/" "${CMAKE_SOURCE_DIR}/../../")
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/deps")
find_library(gtest_path libgtest.a)
find_library(gtest_main_path libgtest_main.a)

add_executable(fs-test ../Yaml.cpp YamlTest.cpp)
target_link_libraries(fs-test PRIVATE ${gtest_path} ${gtest_main_path} -lpthread -lyaml-cpp)
