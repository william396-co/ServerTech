

set(BUILD_VERSION "1.0.0")
set(BUILD_NAME "Client")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

file(GLOB DIR_SRCS *.h *.cpp)


source_group("" FILE ${DIR_SRCS})

add_executable(Client ${DIR_SRCS})

target_link_libraries(Client utils)
target_link_libraries(Client pthread m)


if((CMAKE_BUILD_TYPE MATCHES "Release") OR (CMAKE_BUILD_TYPE MATCHES "Beta"))
		target_link_libraries(Client jemalloc)
endif()

set_property(TARGET Client PROPERTY FOLDER "gameapps")
