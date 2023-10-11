#include <chrono>
#include <iostream>
#include <future>

constexpr auto tenMill = 10000000;

class MySingleton
{
public:
    static MySingleton & getInstance()
    {
        static MySingleton instance;
        volatile int dummy {};
        return instance;
    }

private:
    MySingleton() = default;
    ~MySingleton() = default;
    MySingleton( MySingleton const & ) = delete;
    MySingleton & operator=( MySingleton const & ) = delete;
};

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
