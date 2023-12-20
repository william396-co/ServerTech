file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(sync ${DIR_SRCS})
target_link_libraries(sync pthread)
