set(LIBINCLUDE_SRC print.h print.cpp)

add_library(include STATIC ${LIBINCLUDE_SRC})

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(NOT WIN32)
target_link_libraries(include pthread)
endif()
