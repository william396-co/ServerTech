set(LIBINCLUDE_SRC print.h print.cpp)

add_library(include STATIC ${LIBINCLUDE_SRC})
target_link_libraries(include pthread)
