file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(UnderstandDecltype ${DIR_SRCS})
