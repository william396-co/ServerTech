file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(hello_co_await ${DIR_SRCS})
#target_compile_options(hello_co_await routine PRIVATE -fcoroutines)
