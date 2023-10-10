#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <atomic>
#include <future>

constexpr auto size = 100000000;

constexpr auto fir = 25000000;
constexpr auto sec = 50000000;
constexpr auto thi = 75000000;

std::mutex myMtx;

void sumUp( std::promise<unsigned long long> && prom, std::vector<int> const & val, unsigned long long beg,
    unsigned long long end )
{
    unsigned long long tmpSum {};
    for ( auto it = beg; it != end; ++it ) {
        tmpSum += val[it];
    }
    prom.set_value( tmpSum );
}

int main()
{

    std::vector<int> randValues;
    randValues.reserve( size );

    std::random_device rd;
    std::mt19937 engine( rd() );
    std::uniform_int_distribution dist( 1, 10 );
    for ( auto i = 0; i != size; ++i ) {
        randValues.emplace_back( dist( engine ) );
    }

    auto start = std::chrono::steady_clock::now();

    std::promise<unsigned long long> p1;
    std::promise<unsigned long long> p2;
    std::promise<unsigned long long> p3;
    std::promise<unsigned long long> p4;

    auto f1 = p1.get_future();
    auto f2 = p2.get_future();
    auto f3 = p3.get_future();
    auto f4 = p4.get_future();

    std::thread t1( sumUp, std::move( p1 ), std::cref( randValues ), 0, fir );
    std::thread t2( sumUp, std::move( p2 ), std::cref( randValues ), fir, sec );
    std::thread t3( sumUp, std::move( p3 ), std::cref( randValues ), sec, thi );
    std::thread t4( sumUp, std::move( p4 ), std::cref( randValues ), thi, size );

    auto sum = f1.get() + f2.get() + f3.get() + f4.get();

    std::chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
    std::cout << "Time for addition " << dur.count() << " seconds" << std::endl;
    std::cout << "Result: " << sum << std::endl;

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
