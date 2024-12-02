cmake_minimum_required(VERSION 3.9)

project(hello_boost)

set(CMAKE_CXX_STAND 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		-Werror
		-O0
		)

add_subdirectory(boost_version)
