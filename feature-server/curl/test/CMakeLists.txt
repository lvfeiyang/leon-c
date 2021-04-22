cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/deps" "/home/lxj/proxygen/proxygen/_build/lib/cmake" "/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
find_package(proxygen REQUIRED)
find_library(gtest_path libgtest.a)
find_library(gtest_main_path libgtest_main.a)
find_package(Protobuf REQUIRED)

add_executable(fs-test ../CurlClient.cpp CurlClientTest.cpp ../../util/pbdata/serv-msg.pb.cc ../HttpCli.cpp)
target_link_libraries(
	fs-test PUBLIC ${gtest_path} ${gtest_main_path} ${Protobuf_LIBRARIES}
	proxygen::proxygen
)
target_include_directories(
	fs-test PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>
)
