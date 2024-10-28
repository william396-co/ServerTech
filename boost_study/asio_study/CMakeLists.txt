cmake_minimum_required(VERSION 3.9)

project(boost_study)

set(CMAKE_CXX_STAND 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		-Werror
		-O0
		)

add_subdirectory(timer_sync)
add_subdirectory(hello_boost)
add_subdirectory(timer_async)
add_subdirectory(binding_arg_completion_handler)
add_subdirectory(memfn_completion_handler)
add_subdirectory(sync_completion_handler_in_multithread)

add_subdirectory(sync_tcp_client)
add_subdirectory(sync_tcp_server)
add_subdirectory(async_tcp_server)
