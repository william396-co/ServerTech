#include <future>
#include <iostream>
#include <thread>
#include <utility>

void product( std::promise<int> && intPromise, int a, int b )
{
    intPromise.set_value( a * b );
}

struct Div
{
    void operator()( std::promise<int> && intPromise, int a, int b ) const
    {
        intPromise.set_value( a / b );
    }
};

void test_demo()
{
    int a = 20;
    int b = 10;

    // define the promise
    std::promise<int> prodPromise;
    std::promise<int> divPromise;

    // get the future
    std::future<int> prodResult = prodPromise.get_future();
    std::future<int> divResult = divPromise.get_future();

    // calculate the result in a separate thread
    std::thread prodThread( product, std::move( prodPromise ), a, b );

    Div div;
    std::thread divThread( div, std::move( divPromise ), a, b );

    // get the result
    std::cout << "20 * 10 = " << prodResult.get() << "\n";
    std::cout << "20 / 10 = " << divResult.get() << "\n";
    std::cout << " finished get result\n";

    prodThread.detach();
    divThread.detach();
}

int main()
{
    //  future.get() /wait() made the asynchronize operations synchronisation
    test_demo();
    std::cout << "finished main\n";
    return 0;
}
