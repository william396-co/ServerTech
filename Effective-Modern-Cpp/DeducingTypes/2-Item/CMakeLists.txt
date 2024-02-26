file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(UnderstandAutoTypeDeduction ${DIR_SRCS})
