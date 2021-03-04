cmake_minimum_required(VERSION 3.10)

project (http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/lib/cmake" "/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
find_package(proxygen REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/")

add_executable(http-server main.cpp
	service/Service.cpp)
target_link_libraries(http-server PRIVATE proxygen::proxygen proxygen::proxygenhttpserver)
