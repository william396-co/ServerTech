#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#include "threadsafe_queue.h"
#include "random_util.h"
#include "async_example.h"
#include "future_promise.h"
#include "time_limit.h"

int find_the_answer_to_ltuae()
{
    // std::this_thread::sleep_for( std::chrono::milliseconds { 1000 } );
    std::cout << __PRETTY_FUNCTION__ << "\n";
    return 42;
}

void do_other_stuff()
{
    std::this_thread::sleep_for( std::chrono::seconds { 2 } );
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void return_value_from_async_task()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::future<int> the_answer = std::async( find_the_answer_to_ltuae );
    do_other_stuff();
    std::cout << "The answer is " << the_answer.get() << "\n";
}

threadsafe_queue<std::string> data_queue;

void do_push()
{
    while ( true ) {
        auto str = random_string( 10, 20 );
        std::cout << __PRETTY_FUNCTION__ << "(" << str << ")\n";
        data_queue.push( str );
    }
}

void do_wait_and_pop()
{
    while ( true ) {
        auto str = data_queue.wait_and_pop();
        std::cout << __PRETTY_FUNCTION__ << "(" << str << ")\n";
    }
}

void condition_variable_test()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::vector<std::jthread> producer;
    for ( auto i = 0; i != 3; ++i ) {
        producer.emplace_back( do_push );
    }
    std::jthread consumer { do_wait_and_pop };
}

int main()
{

    chrono_example();
//    shared_future_example();
//    future_exception_example();
    async_example();
    async_example2();
    return_value_from_async_task();
    // condition_variable_test();

    return 0;
}
