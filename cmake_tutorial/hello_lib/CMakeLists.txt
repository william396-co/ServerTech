cmake_minimum_required(VERSION 3.9)

project(LIBHELLO)

# CXX standard
set(CMAKE_CXX_STANDARD 20)
# CXX compiler flags
set(CXX_FLAGS -Wall -Werror -fPIC)

add_subdirectory(src bin)
