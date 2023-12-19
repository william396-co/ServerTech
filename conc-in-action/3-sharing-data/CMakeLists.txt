file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(sharing_data ${DIR_SRCS})
target_link_libraries(sharing_data pthread)
