cmake_minimum_required(VERSION 3.9)

project(TIMEWHEEL)

if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug")
endif()


set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		)


#add_subdirectory(timerWheel)
add_subdirectory(xtime_demo)
