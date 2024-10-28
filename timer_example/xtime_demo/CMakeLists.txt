file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})
add_executable(xtime_demo ${DIR_SRCS})
