#include <iostream>
#include <string>
#include <mutex>
#include "src/ThreadPool.h"
#include "src/util.h"

std::mutex out_mtx;

void print( int a, double b, const char * c, std::string const & d )
{
    std::unique_lock lock( out_mtx );
    std::cout << "[" << std::this_thread::get_id() << "]" << a << " " << b << " " << c << " " << d << std::endl;
}

void test()
{
    std::unique_lock lock( out_mtx );
    std::cout << "[" << std::this_thread::get_id() << "] help" << std::endl;
}

int main()
{
    ThreadPool * pool = new ThreadPool();
    for ( int i = 0; i != 30; ++i ) {
        Task func = std::bind( print, 1, 3.14, "hello", std::string( "world" ) );
        pool->add( func );

        func = test;
        pool->add( func );
    }
    delete pool;
}
