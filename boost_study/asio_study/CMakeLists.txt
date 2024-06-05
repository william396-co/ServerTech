cmake_minimum_required(VERSION 3.9)

project(boost_study)

set(CMAKE_CXX_STAND 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		-Werror
		-O0
		)

#add_subdirectory(timer_sync)
add_subdirectory(hello_boost)
#add_subdirectory(timer_async)
