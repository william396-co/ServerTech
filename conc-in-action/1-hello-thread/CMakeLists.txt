file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(hello_thread ${DIR_SRCS})
target_link_libraries(hello_thread pthread)
