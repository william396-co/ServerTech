cmake_minimum_required(VERSION 3.9)

project(CONCURRENCY_IN_ACTION CXX)


set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


add_subdirectory(1-hello-thread)
add_subdirectory(2-managing-threads)
add_subdirectory(3-sharing-data)
add_subdirectory(4-sync)
add_subdirectory(atm_example)
add_subdirectory(5-memo-model-atomic)
