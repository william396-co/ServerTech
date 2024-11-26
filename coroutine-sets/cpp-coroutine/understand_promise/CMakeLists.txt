file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(understand_promise ${DIR_SRCS})
