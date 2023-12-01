cmake_minimum_required(VERSION 3.8)

project(coroutines_example C CXX)

set(CMAKE_CXX_STANDARD 20)

message("CXX STANDARD ", ${CMAKE_CXX_STANDADR})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++2a")

add_executable(co_await_example src/co_await_example.cpp)
#target_compile_options(co_await_example PRIVATE -fcoroutines)
