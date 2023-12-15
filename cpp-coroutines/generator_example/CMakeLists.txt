file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(generator_example ${DIR_SRCS})
#target_link_libraries(generator_example pthread)
