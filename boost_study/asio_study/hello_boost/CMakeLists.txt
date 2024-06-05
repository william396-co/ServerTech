file(GLOB DIR_SRCS *.h *.cpp)

source_group("" FILE ${DIR_SRCS})

add_executable(hello_boost ${DIR_SRCS})

set(BOOST_ROOT /usr/local/include/boost)
set(BOOST_LIBRARYDIR /usr/local/lib)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_DEBUG ON)

find_package(Boost  REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(hello_boost Boost::filesystem Boost::system)
else()
  message(STATUS "Boost NOT Found !")
endif(Boost_FOUND)





