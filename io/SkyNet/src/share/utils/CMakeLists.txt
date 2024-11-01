file(GLOB DIR_SRCS *.h *.cpp *.h *.cc *.c)


source_group("" FILES ${DIR_SRCS})

add_library(utils ${DIR_SRCS})
set_property(TARGET utils PROPERTY FOLDER "share")
