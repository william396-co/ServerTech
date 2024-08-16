cmake_minimum_required(VERSION 3.9)

project(coroutine_example)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS 
		-ggdb
		-Wall 
		-Wextra
		-Werror
		-O0
	)

add_subdirectory(constexpr-lambda)
add_subdirectory(exception-specification-in-type-system)
add_subdirectory(factorial-lambda)
add_subdirectory(capture-this)
add_subdirectory(capture-variable)
add_subdirectory(invoke-lambda)
add_subdirectory(fold-express-lambda)
