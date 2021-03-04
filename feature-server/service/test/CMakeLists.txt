cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})
include_directories("/home/lxj/proxygen/proxygen/_build/deps/include/" "${CMAKE_SOURCE_DIR}/../../")
list(APPEND CMAKE_PREFIX_PATH
	"/home/lxj/proxygen/proxygen/_build/deps"
	"/home/lxj/proxygen/proxygen/_build/lib/cmake"
	"/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
find_library(gtest_path libgtest.a)
find_library(gtest_main_path libgtest_main.a)
find_package(Protobuf REQUIRED)
find_package(proxygen REQUIRED)
message("leon_debug ${gtest_path}")

add_executable(fs-test
	../../util/pbdata/serv-msg.pb.cc
	../Msg.cpp
	ServiceTest.cpp)
target_link_libraries(fs-test PRIVATE ${gtest_path} ${gtest_main_path} ${Protobuf_LIBRARIES} proxygen::proxygen) # -lpthread
