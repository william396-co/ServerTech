file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(managing_threads ${DIR_SRCS})
target_link_libraries(managing_threads pthread)
