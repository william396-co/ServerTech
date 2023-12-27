file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(memo_model ${DIR_SRCS})
target_link_libraries(memo_model pthread)
