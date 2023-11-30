cmake_minimum_required(VERSION 3.9)

project(HELLO CXX)


set(CMAKE_CXX_STANDARD 17)

# include 
include_directories(include)
include_directories(lib)

# add_subdirectory
add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(src)
