cmake_minimum_required(VERSION 3.9)

project(coroutine_example)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS 
		-ggdb
		-Wall 
		-Wextra
		-Werror
		-O0
	)

add_subdirectory(func)
add_subdirectory(functor)
add_subdirectory(functor_with_member)
add_subdirectory(functor_capture)

