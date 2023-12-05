file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(co_await_example ${DIR_SRCS})
#target_compile_options(co_await_example PRIVATE -fcoroutines)
