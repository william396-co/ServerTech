#include <iostream>
#include <string>
#include "src/ThreadPool.h"
#include "src/util.h"

void print( int a, double b, const char * c, std::string const & d )
{
    std::cout << a << b << c << d << "\n";
}

void test()
{
    std::cout << "help\n";
}

int main()
{
    ThreadPool * pool = new ThreadPool();
    Task func = std::bind( print, 1, 3.14, "hello", std::string( "world" ) );
    pool->add( func );

    func = test;
    pool->add( func );
    delete pool;
}
