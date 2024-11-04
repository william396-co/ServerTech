file(GLOB DIR_SRCS *.h *.cpp *.h *.cc *.c)

source_group("" FILES ${DIR_SRCS})

add_library(io ${DIR_SRCS})
target_link_libraries(io utils)

set_property(TARGET io PROPERTY FOLDER "share")
