cmake_minimum_required(VERSION 3.9)

project(meta_prog)

set(CMAKE_CXX_STAND 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		-Werror
		-O0
		)

add_subdirectory(type_associations)
add_subdirectory(metafunctions)
add_subdirectory(numerical_metafunctions)
