#include <memory>
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

class A
{
public:
    explicit A( int n )
        : data { n }
    {
        std::cout << "[" << std::this_thread::get_id() << "]" << __PRETTY_FUNCTION__ << "(" << data << ")\n";
    }
    ~A()
    {
        std::cout << "[" << std::this_thread::get_id() << "]" << __PRETTY_FUNCTION__ << "(" << data << ")\n";
    }

private:
    int data;
};

int main()
{

    std::shared_ptr<A> ptr = std::make_shared<A>( 2023 );

    std::vector<std::thread> threads;
    for ( int i = 0; i != 10; ++i ) {
        threads.emplace_back( [&ptr, &i] {
            // auto localptr = std::make_shared<A>( 2025 + i );
            //            std::atomic_store( &ptr, localptr );
            ptr = std::make_shared<A>( 2025 + i );
        } );
    }

    std::for_each( threads.begin(), threads.end(), std::mem_fn( &std::thread::join ) );

    return 0;
}
