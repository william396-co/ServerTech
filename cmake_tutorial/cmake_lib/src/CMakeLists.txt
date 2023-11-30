set(CALL_LIB_SRC main.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(call_hello ${CALL_LIB_SRC})
target_link_libraries(call_hello hello)

