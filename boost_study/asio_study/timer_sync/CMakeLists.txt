file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(timer_sync ${DIR_SRCS})

find_package(Boost COMPONENTS asio system filesystem REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(hello_boost Boost::asio Boost::filesystem Boost::system)
else()
  message(STATUS "Boost NOT Found !")
endif(Boost_FOUND)





