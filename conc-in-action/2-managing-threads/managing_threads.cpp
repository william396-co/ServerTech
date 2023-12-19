#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>

#include "thread_guard.h"
#include "scoped_thread.h"
#include "joining_thread.h"
#include "parallel_accumulate.h"

void do_work( int i, std::string const & msg )
{
    std::cout << msg << " : " << __PRETTY_FUNCTION__ << "(" << i << ")\n";
}

void run_thread_background()
{
    std::thread t( [] {
        std::cout << "thread running in background\n";
    } );
    t.detach();
}

int main()
{
    std::thread t( [] {
        std::cout << "thread guard test\n";
    } );
    run_thread_background();
    {
        scoped_thread sg { std::thread( [] {
            std::cout << "scoped thread test\n";
        } ) };
    }
    thread_guard g( t );

    {
        std::vector<joining_thread> threads;
        for ( auto i = 0; i != 10; ++i ) {
            threads.emplace_back( do_work, i, "joining_thread" );
        }
    }
    {
        std::vector<std::thread> threads;
        for ( auto i = 0; i != 10; ++i ) {
            threads.emplace_back( do_work, i, "std::thread" );
        }

        std::for_each( threads.begin(), threads.end(), std::mem_fn( &std::thread::join ) );
    }
    {
        std::vector<std::jthread> threads;
        for ( auto i = 0; i != 10; ++i ) {
            threads.emplace_back( do_work, i, "std::jthread" );
        }
    }

    std::vector<int> vi( 10000 );
    for ( auto i = 0; i != vi.size(); ++i ) {
        vi.emplace_back( i * 3 );
    }
    auto result = parallel_accumulate( vi.begin(), vi.end(), 10 );
    std::cout << "result= " << result << "\n";
    return 0;
}
