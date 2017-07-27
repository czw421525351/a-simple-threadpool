cmake_minimum_required(VERSION 3.0)
project(a-simple-threadpool)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_FLAGS -std=c++11)
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")


add_executable(threadpool_test condition.cpp threadpool.cpp threadpool_test.cpp)
target_link_libraries(threadpool_test pthread)

