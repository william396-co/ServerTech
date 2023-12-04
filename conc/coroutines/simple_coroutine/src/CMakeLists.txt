
file(GLOB DIR_SRC *.h *.cpp)

source_group("" FILE ${FID_SRC})

add_executable(simple_coroutine ${DIR_SRCS})

