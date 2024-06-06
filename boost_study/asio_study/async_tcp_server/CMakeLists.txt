file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(async_tcp_server ${DIR_SRCS})

find_package(Boost COMPONENTS  system filesystem REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(async_tcp_server Boost::filesystem Boost::system pthread)
else()
  message(STATUS "Boost NOT Found !")
endif(Boost_FOUND)





