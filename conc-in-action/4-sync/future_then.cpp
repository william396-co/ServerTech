#include "future_then.h"

// #include <experimental/future>
#include <future>

#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>

int find_the_answer()
{
    std::cout << __PRETTY_FUNCTION__ << " start\n";
    std::this_thread::sleep_for( std::chrono::seconds { 3 } );
    std::cout << __PRETTY_FUNCTION__ << " end\n";
    return 42;
}

void find_the_question()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void future_then_example()
{
    /*std::experimental::future<int> find_the_answer;
   auto fut = find_the_answer();
       auto fut2 = fut.then( find_the_question );

   std::cout << " after fut2 \n";

   assert( !fut.valid() );
     assert( fut2.valid() );
     */
}

