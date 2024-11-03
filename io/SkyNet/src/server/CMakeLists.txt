

set(BUILD_VERSION "1.0.0")
set(BUILD_NAME "server")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

file(GLOB DIR_SRCS *.h *.cpp)


source_group("" FILE ${DIR_SRCS})

add_executable(server ${DIR_SRCS})

target_link_libraries(server utils)
target_link_libraries(server pthread m)


if((CMAKE_BUILD_TYPE MATCHES "Release") OR (CMAKE_BUILD_TYPE MATCHES "Beta"))
		target_link_libraries(server jemalloc)
endif()

set_property(TARGET server PROPERTY FOLDER "gameapps")
