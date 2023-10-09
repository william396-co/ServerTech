#include <future>
#include <iostream>
#include <thread>
#include <utility>

void doTheWork()
{
    std::cout << "Processing sharead data.\n";
}

void waitingForWork( std::future<void> && fut )
{
    std::cout << "Worker: Waiting for work.\n";
    fut.wait();
    doTheWork();
    std::cout << "Work done.\n";
}

void setDataReady( std::promise<void> && prom )
{
    std::cout << "Sender: Data is ready.\n";
    prom.set_value();
}

int main()
{

    std::promise<void> sendReady;
    auto fut = sendReady.get_future();

    std::thread t1( waitingForWork, std::move( fut ) );
    std::thread t2( setDataReady, std::move( sendReady ) );

    t1.join();
    t2.join();

    return 0;
}
