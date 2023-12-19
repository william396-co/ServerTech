#include <chrono>
#include <iostream>

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

int main()
{
    auto begin = std::chrono::system_clock::now();
    const auto fourtyMill = 4 * tenMill;
    for ( auto i = 0; i != fourtyMill; ++i ) {
        MySingleton::getInstance();
    }
    auto end = std::chrono::system_clock::now() - begin;
    std::cout << std::chrono::duration<double>( end ).count() << "\n";
}
