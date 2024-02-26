cmake_minimum_required(VERSION 3.9)

project(EffectiveModernC++)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(DeducingTypes/1-Item)
add_subdirectory(DeducingTypes/2-Item)
