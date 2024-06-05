cmake_minimum_required(VERSION 3.9)

project(boost_study)

set(CMAKE_CXX_STAND 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CXX_FLAGS
		-ggdb
		-Wall
		-Wextra
		-Werror
		-O0
		)

set(BOOST_ROOT /usr/include/boost) # 设置boost库搜索路径
set(Boost_NO_SYSTEM_PATHS ON) # 只搜索上语句设置的搜索路径

#find_package(Boost COMPONENTS regex system date_time program_options asio)

find_package(Boost REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    
	MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")

    target_link_libraries (foo ${Boost_LIBRARIES})
endif()

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time)
add_subdirectory(timer_sync)
#add_subdirectory(timer_async)
