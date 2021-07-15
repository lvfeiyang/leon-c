cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Arrow REQUIRED)

# message("leon_debug ${ARROW_VERSION}")

add_executable(hdfs-test hdfs-test.cpp)
target_link_libraries(hdfs-test PRIVATE arrow_static)
