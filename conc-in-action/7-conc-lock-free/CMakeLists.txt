file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(conc_lock_free ${DIR_SRCS})
target_link_libraries(conc_lock_free pthread)
