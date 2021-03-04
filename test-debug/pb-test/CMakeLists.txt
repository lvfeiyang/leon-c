cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Protobuf REQUIRED)
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pbdata/test.proto)
# message("leon_debug: ${PROTO_SRCS} ${PROTO_HDRS} ${Protobuf_INCLUDE_DIRS} ${Protobuf_LIBRARIES}")

add_executable(pb-test pb-test.cpp pbdata/test.pb.cc)
target_link_libraries(pb-test ${Protobuf_LIBRARIES})
