cmake_minimum_required (VERSION 3.8)

project (sync_example C CXX)

set(CXX_STANDARD 11)
add_executable (sync_example "sync_example.cpp" "sync_example.h")
if(NOT _WIN32)
target_link_libraries(sync_example pthread)
endif()

