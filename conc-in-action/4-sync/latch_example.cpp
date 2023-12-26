#include "latch_example.h"

#include <thread>
#include <latch>
#include <iostream>
#include <future>
#include <chrono>
#include <random>
#include <mutex>

std::mutex os_mtx;

void process_data( unsigned thread_cnt )
{
    std::cout << __PRETTY_FUNCTION__ << " thread_count :" << thread_cnt << "\n";
}

void latch_example()
{
    unsigned const thread_count = 5;
    std::latch done( thread_count );
    std::vector<std::future<void>> threads;
    for ( size_t i = 0; i != thread_count; ++i ) {
        threads.push_back( std::async( std::launch::async, [&, i] {
            auto seconds = std::chrono::seconds { rand() % 10 };
            std::this_thread::sleep_for( seconds );
            std::lock_guard lock( os_mtx );
            std::cout << "idx :" << i << " thread [" << std::this_thread::get_id() << "] run " << seconds << " \n";
            done.count_down();
        } ) );
    }
    done.wait();
    process_data( thread_count );
}

