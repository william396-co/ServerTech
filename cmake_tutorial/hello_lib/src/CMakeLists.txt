cmake_minimum_required(VERSION 3.9)

set(LIBHELLO_SRC hello.cpp)

add_library(hello_static STATIC ${LIBHELLO_SRC})
set_target_properties(hello_static PROPERTIES OUTPUT_NAME "hello")
set_target_properties(hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)


add_library(hello SHARED ${LIBHELLO_SRC})
set_target_properties(hello PROPERTIES OUTPUT_NAME "hello")
set_target_properties(hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set so version
set_target_properties(hello PROPERTIES VERSION 1.2 SOVERSION 1)


# include file allocate
install(FILES hello.h DESTINATION include/hello)

# .so .a 
install(TARGETS hello hello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION bin)

