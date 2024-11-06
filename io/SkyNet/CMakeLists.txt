cmake_minimum_required(VERSION 3.8)

project(Skynet C CXX)

if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT PROJECT_OUTPUT_PATH)
		set(PROJECT_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
endif()

# CXX FLAGS
set(CXX_FLAGS
	-ggdb
	-Wall
	-Wextra
	-Werror
	)

set(CMAKE_CXX_STANDARD 17)


#path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/archive)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake; ${CMAKE_MODULE_PATH}")


include_directories(src) 
include_directories(src/share)

# link
link_directories(${PROJECT_SOURCE_DIR}/lib)

# public lib
add_subdirectory(src/share/utils)
add_subdirectory(src/share/io)

# sub project
add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(src/ThreadPoolTest)
add_subdirectory(src/test)

