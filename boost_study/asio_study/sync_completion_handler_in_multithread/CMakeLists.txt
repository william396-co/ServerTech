file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(sync_completion_handler_in_multithread ${DIR_SRCS})

find_package(Boost COMPONENTS  system filesystem REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(sync_completion_handler_in_multithread Boost::filesystem Boost::system pthread)
else()
  message(STATUS "Boost NOT Found !")
endif(Boost_FOUND)





