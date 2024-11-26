file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(resumable_thing ${DIR_SRCS})
#target_link_libraries(resumable_thing pthread)
