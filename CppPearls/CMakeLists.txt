cmake_minimum_required(VERSION 3.9)

project(CppPearls)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS 
		-ggdb
		-Wall 
		-Wextra
		-Werror
		-O0
	)

add_subdirectory(using_crtp)
add_subdirectory(basic_of_macros)
add_subdirectory(basic_template)
