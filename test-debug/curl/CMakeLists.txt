# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/lib/cmake" "/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
find_package(proxygen REQUIRED)

add_library(proxygencurl STATIC CurlClient.cpp)
target_include_directories(
	proxygencurl PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(proxygencurl PUBLIC proxygen::proxygen)

add_executable(proxygen_curl CurlClientMain.cpp)
target_link_libraries(
	proxygen_curl PUBLIC
	proxygencurl proxygen::proxygen
)
