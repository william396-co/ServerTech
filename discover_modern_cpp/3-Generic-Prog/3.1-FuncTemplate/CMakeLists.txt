file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})
add_executable(func_tpl ${DIR_SRCS})
