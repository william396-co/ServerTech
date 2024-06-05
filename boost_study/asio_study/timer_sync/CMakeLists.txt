file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

include_directories(${Boost_INCLUDE_DIRS})

add_executable(timer_sync ${DIR_SRCS})

target_link_libraries(timer_sync ${Boost_LIBRARIES}	)


