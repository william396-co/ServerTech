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

add_subdirectory(hello_co_await)
#add_subdirectory(simple_coroutine)
add_subdirectory(hello_co_yield)
add_subdirectory(hello_co_return)
add_subdirectory(hello_coroutine)
add_subdirectory(resumable_thing)
add_subdirectory(fib_gen)
add_subdirectory(switch_2_new_thread)
add_subdirectory(generator_example)
add_subdirectory(understand_co_await)
add_subdirectory(understand_promise)
