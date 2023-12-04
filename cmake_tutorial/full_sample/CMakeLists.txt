cmake_minimum_required(VERSION 3.9)

project(use_lib CXX)

set(CXX_FLAGS -Wall -Werror -Og)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src bin)
