cmake_minimum_required(VERSION 3.10)

project (feature-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH
	"/home/lxj/proxygen/cmake"
	"/home/lxj/proxygen/build/fbcode_builder/CMake"
	${CMAKE_MODULE_PATH})
list(APPEND CMAKE_PREFIX_PATH "/home/lxj/proxygen/proxygen/_build/lib/cmake" "/home/lxj/proxygen/proxygen/_build/deps/lib/cmake")
find_package(proxygen REQUIRED)
find_package(Protobuf REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/")

add_executable(feature-server main.cpp
	service/Service.cpp service/Msg.cpp
	util/Yaml.cpp
	util/pbdata/serv-msg.pb.cc
	db/RedisDB.cpp
	cache/Cache.cpp)
target_link_libraries(feature-server PRIVATE proxygen::proxygen proxygen::proxygenhttpserver ${Protobuf_LIBRARIES} -lhiredis -lyaml-cpp)
