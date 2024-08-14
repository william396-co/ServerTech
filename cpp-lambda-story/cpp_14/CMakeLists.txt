cmake_minimum_required(VERSION 3.9)

project(coroutine_example)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS 
		-ggdb
		-Wall 
		-Wextra
		-Werror
		-O0
	)

add_subdirectory(default-parameter)
#add_subdirectory(auto-return)
add_subdirectory(return-function)
add_subdirectory(auto-return-type-deduc-for-lambda)
add_subdirectory(capture-initialiser)
add_subdirectory(capture-moveable-only)
add_subdirectory(optimisation)
add_subdirectory(capture-mem-var)
add_subdirectory(generic-lambda)
add_subdirectory(generic-variadic-lambda)
add_subdirectory(perfect-forwarding-generic-lambda)
add_subdirectory(deducing-correct-type)
add_subdirectory(replacing-with-stdbind)
add_subdirectory(function-composition)
