file(GLOB_RECURSE skynet_sources ${PROJECT_SOURCE_DIR}/src/shared/*.cpp)

add_library(skynet_shared SHARED ${skynet_sources})
