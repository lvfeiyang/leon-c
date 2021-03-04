cmake_minimum_required(VERSION 3.10)

project(leon-echo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})

list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/lib/cmake" "/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
# list(APPEND CMAKE_MODULE_PATH "/var/lxj/wp/cpp/facebook/proxygen/build/fbcode_builder/CMake")
# list(APPEND CMAKE_PREFIX_PATH "/var/lxj/wp/cpp/facebook/proxygen/proxygen/_build" "/var/lxj/wp/cpp/facebook/proxygen/proxygen/_build/deps")

find_package(proxygen REQUIRED)

add_executable(leon-echo EchoHandler.cpp EchoServer.cpp)
# target_compile_options(
# 	leon-echo
# 	PRIVATE
# 		${_PROXYGEN_COMMON_COMPILE_OPTIONS}
# )
message("leon_debug: ${proxygen_INCLUDE_DIRS}")
target_link_libraries(leon-echo PRIVATE proxygen::proxygen proxygen::proxygenhttpserver)
