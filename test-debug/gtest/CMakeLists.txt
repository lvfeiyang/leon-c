cmake_minimum_required(VERSION 3.10)

project (leon-gtest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/home/lxj/proxygen/proxygen/_build/deps/include/")
# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/home/lxj/proxygen/proxygen/_build/deps")
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/deps")
find_library(gtest_path libgtest.a)
find_library(gtest_main_path libgtest_main.a)
message("leon_debug: ${leon-gtest_SOURCE_DIR}")

add_executable(leon-gtest main.cpp)
target_link_libraries(leon-gtest ${gtest_path} ${gtest_main_path} -lpthread)
