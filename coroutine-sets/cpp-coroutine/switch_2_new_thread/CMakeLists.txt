file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(switch_2_new_thread ${DIR_SRCS})
target_link_libraries(switch_2_new_thread pthread)
