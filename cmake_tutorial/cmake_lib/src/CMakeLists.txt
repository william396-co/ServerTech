set(CALL_LIB_SRC main.cpp)

add_executable(call_hello ${CALL_LIB_SRC})

target_link_libraries(call_hello hello)

