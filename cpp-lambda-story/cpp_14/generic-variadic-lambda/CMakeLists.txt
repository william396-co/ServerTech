file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(generic-variadic-lambda ${DIR_SRCS})
