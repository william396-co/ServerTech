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
add_subdirectory(advanced_template)
add_subdirectory(cond_loop_recurision)
add_subdirectory(const_expression)
add_subdirectory(fold_expression)
add_subdirectory(GMP_Library)
add_subdirectory(principle_of_macro_recursion)
add_subdirectory(source_code_injection)
add_subdirectory(static_reflection)
