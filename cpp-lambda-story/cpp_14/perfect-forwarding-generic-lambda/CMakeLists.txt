file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(perfect-forwarding-generic-lambda ${DIR_SRCS})
