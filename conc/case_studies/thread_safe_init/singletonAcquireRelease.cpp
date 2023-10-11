#include <chrono>
#include <iostream>
#include <future>
#include <mutex>
#include <atomic>
#include <thread>

constexpr auto tenMill = 10000000;

class MySingleton
{
public:
    static MySingleton & getInstance()
    {
        MySingleton * sin = instance.load( std::memory_order_acquire );
        if ( !sin ) {
            std::lock_guard lock( myMutex );
            sin = instance.load( std::memory_order_relaxed );
            if ( !sin ) {
                sin = new MySingleton();
                instance.store( sin, std::memory_order_release );
            }
        }
        volatile int dummy {};
        return *instance;
    }

private:
    MySingleton() = default;
    ~MySingleton() = default;
    MySingleton( MySingleton const & ) = delete;
    MySingleton & operator=( MySingleton const & ) = delete;

    static std::atomic<MySingleton *> instance;
    static std::mutex myMutex;
};

std::atomic<MySingleton *> MySingleton::instance;
std::mutex MySingleton::myMutex;

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
