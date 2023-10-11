#include <chrono>
#include <iostream>
#include <future>
#include <mutex>
#include <thread>

constexpr auto tenMill = 10000000;

class MySingleton
{
public:
    static MySingleton & getInstance()
    {
        std::call_once( initInstanceFlag, &MySingleton::initSingleton );
        volatile int dummy {};
        return *instance;
    }

private:
    MySingleton() = default;
    ~MySingleton() = default;
    MySingleton( MySingleton const & ) = delete;
    MySingleton & operator=( MySingleton const & ) = delete;
    static MySingleton * instance;
    static std::once_flag initInstanceFlag;

    static void initSingleton()
    {
        instance = new MySingleton();
    }
};

MySingleton * MySingleton::instance = nullptr;
std::once_flag MySingleton::initInstanceFlag;

std::chrono::duration<double> getTime()
{
    auto begin = std::chrono::system_clock::now();
    for ( auto i = 0; i != tenMill; ++i ) {
        MySingleton::getInstance();
    }
    return std::chrono::system_clock::now() - begin;
}

int main()
{
    auto f1 = std::async( std::launch::async, getTime );
    auto f2 = std::async( std::launch::async, getTime );
    auto f3 = std::async( std::launch::async, getTime );
    auto f4 = std::async( std::launch::async, getTime );

    auto total = f1.get() + f2.get() + f3.get() + f4.get();

    std::cout << total.count() << "\n";
}
