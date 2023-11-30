cmake_minimum_required(VERSION 3.9)

set(LIBHELLO_SRC hello.cpp)

#add_library(hello_so STATIC ${LIBHELLO_SRC})
add_library(hello SHARED ${LIBHELLO_SRC})
target_link_libraries(hello include)
