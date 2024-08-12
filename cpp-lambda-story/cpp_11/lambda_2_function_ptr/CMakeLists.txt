file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(lambda_2_function_ptr ${DIR_SRCS})
