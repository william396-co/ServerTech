file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(atm ${DIR_SRCS})
target_link_libraries(atm pthread)
