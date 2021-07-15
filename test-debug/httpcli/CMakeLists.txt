cmake_minimum_required(VERSION 3.10)
project(http-cli)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(cpprestsdk REQUIRED)

add_executable(http-cli httpcli.cpp)
target_link_libraries(http-cli PRIVATE cpprestsdk::cpprest Boost::system)
