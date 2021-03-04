cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/usr/include/mysql-cppconn-8")
add_executable(mysql-test mysql-test-1.cpp)
target_link_libraries(mysql-test -lmysqlcppconn8 -lmysqlcppconn)
