file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(hello_cor ${DIR_SRCS})
#target_compile_options(hello_cor PRIVATE -fcoroutines)
