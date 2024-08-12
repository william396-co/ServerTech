cmake_minimum_required(VERSION 3.9)

project(coroutine_example)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS 
		-ggdb
		-Wall 
		-Wextra
		-Werror
		-O0
	)

add_subdirectory(hello_lambda)
add_subdirectory(type_of_lambda)
add_subdirectory(function_vs_lambda)
add_subdirectory(copy_lambda)
add_subdirectory(lambda_capture)
add_subdirectory(compCounter)
add_subdirectory(capture_global)
add_subdirectory(capture_static)
add_subdirectory(capture_mem_this)
add_subdirectory(return_lambda)
add_subdirectory(capture_const)
add_subdirectory(capture_moveable_only)
add_subdirectory(capture_parameter_pack)
add_subdirectory(return_type)
add_subdirectory(return_literal)
add_subdirectory(lambda_2_function_ptr)
