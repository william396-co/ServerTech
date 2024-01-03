file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(conc_data_struct ${DIR_SRCS})
target_link_libraries(conc_data_struct pthread)
