file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(replacing-with-stdbind ${DIR_SRCS})
