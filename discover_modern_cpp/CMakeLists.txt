cmake_minimum_required(VERSION 3.9)

project(DISCOVERING_MODERN_CPP)


set(CMAKE_CXX_STANDARD 20)
# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# I-Basic
add_subdirectory(1-Basic/1.1-first-prog)

# II-Classes
add_subdirectory(2-Classes/2.1-Members)

# III-Generic-Prog
add_subdirectory(3-Generic-Prog/3.1-FuncTemplate)

