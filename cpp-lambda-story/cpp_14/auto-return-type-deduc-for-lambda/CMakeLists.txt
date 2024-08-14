file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(auto-return-type-deduc-for-lambda ${DIR_SRCS})
