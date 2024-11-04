

set(BUILD_VERSION "1.0.0")
set(BUILD_NAME "client")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

file(GLOB DIR_SRCS *.h *.cpp)


source_group("" FILE ${DIR_SRCS})

add_executable(client ${DIR_SRCS})

target_link_libraries(client utils io)
target_link_libraries(client pthread m)


if((CMAKE_BUILD_TYPE MATCHES "Release") OR (CMAKE_BUILD_TYPE MATCHES "Beta"))
		target_link_libraries(client jemalloc)
endif()

set_property(TARGET client PROPERTY FOLDER "gameapps")
