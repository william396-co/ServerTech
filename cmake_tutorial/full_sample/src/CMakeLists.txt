set(USE_LIB_SRC main.cpp)

# add link libraries header file folder
include_directories(/usr/local/include/hello)
# add link libraries so file folder
link_directories(/usr/local/lib)

# executable
add_executable(main ${USE_LIB_SRC})
# link user defined libraries
target_link_libraries(main libhello.so)



## solution 2
if(TEST)
	file(GLOB DIR_SRCS *.h *.cpp)
	source_group("" FILE ${DIR_SRCS})
	add_executable(main ${DIR_SRCS})
	target_link_libraries(main libhello.so)
endif()
