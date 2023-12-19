#include <atomic>
#include <iostream>
#include <thread>

template<typename T>
T fetch_mult( std::atomic<T> & shared, T mult )
{
    T oldValue = shared.load();
    while ( !shared.compare_exchange_strong( oldValue, oldValue * mult ) )
        ;
    return oldValue;
}

int main()
{

    std::atomic<int> myInt { 5 };
    std::cout << myInt << "\n";

    std::thread t1( [&myInt]( int i ) {
        fetch_mult( myInt, 3 );
        std::cout << "[" << i << "]" << myInt << std::endl;
    },
        1 );
    std::thread t2( [&myInt]( int i ) {
        fetch_mult( myInt, 2 );
        std::cout << "[" << i << "]" << myInt << std::endl;
    },
        2 );

    t1.join();
    t2.join();

    fetch_mult( myInt, 5 );
    std::cout << myInt << "\n";

    return 0;
}
