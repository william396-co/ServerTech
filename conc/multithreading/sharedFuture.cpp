#include <future>
#include <iostream>
#include <thread>
#include <utility>

std::mutex coutMutex;

struct Div
{
    void operator()( std::promise<int> && intPromise, int a, int b ) const
    {
        std::lock_guard lock( coutMutex );
        std::cout << "threadId(" << std::this_thread::get_id() << "): " << __PRETTY_FUNCTION__ << "\n";
        intPromise.set_value( a / b );
    }
};

struct Requestor
{

    void operator()( std::shared_future<int> shaFut )
    {
        std::lock_guard coutGuard( coutMutex );

        // get the thread id
        std::cout << "threadId(" << std::this_thread::get_id() << "): ";
        std::cout << "20/10 = " << shaFut.get() << "\n";
    }
};

int main()
{

    // define promises
    std::promise<int> divPromise;

    // get the futures
    std::shared_future<int> divResult = divPromise.get_future();

    // calculate the result in a separat thread
    Div div;
    std::thread divThread( div, std::move( divPromise ), 20, 10 );

    Requestor req;
    std::thread st1( req, divResult );
    std::thread st2( req, divResult );
    std::thread st3( req, divResult );
    std::thread st4( req, divResult );
    std::thread st5( req, divResult );

    divThread.join();

    st1.join();
    st2.join();
    st3.join();
    st4.join();
    st5.join();

    return 0;
}
