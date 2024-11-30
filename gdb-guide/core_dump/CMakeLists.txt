file(GLOB DIR_SRCS *.h *.cpp *.c *.cc)

source_group("" FILE ${DIR_SRCS})

add_executable(core_dump ${DIR_SRCS})
