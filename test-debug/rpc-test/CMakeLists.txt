cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
# find_package(Protobuf REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

add_executable(rpc-test grpc-server.cpp pbdata/hello.pb.cc pbdata/hello.grpc.pb.cc)
target_link_libraries(rpc-test gRPC::grpc++ gRPC::grpc++_reflection protobuf::libprotobuf)

add_executable(rpc-test-cli grpc-client.cpp pbdata/hello.pb.cc pbdata/hello.grpc.pb.cc)
target_link_libraries(rpc-test-cli gRPC::grpc++ protobuf::libprotobuf)
