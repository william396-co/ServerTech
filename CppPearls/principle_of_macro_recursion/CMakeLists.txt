file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(principle_of_macro_recursion ${DIR_SRCS})
