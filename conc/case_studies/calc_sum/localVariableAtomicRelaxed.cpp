#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <atomic>

constexpr auto size = 100000000;

constexpr auto fir = 25000000;
constexpr auto sec = 50000000;
constexpr auto thi = 75000000;

std::mutex myMtx;

void sumUp( std::atomic<unsigned long long> & sum, std::vector<int> const & val, unsigned long long beg,
    unsigned long long end )
{
    unsigned long long temp {};
    for ( auto it = beg; it != end; ++it ) {
        temp += val[it];
    }
    sum.fetch_add( temp, std::memory_order_relaxed );
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

    std::atomic<unsigned long long> sum = {};
    auto start = std::chrono::steady_clock::now();

    std::thread t1( sumUp, std::ref( sum ), std::cref( randValues ), 0, fir );
    std::thread t2( sumUp, std::ref( sum ), std::cref( randValues ), fir, sec );
    std::thread t3( sumUp, std::ref( sum ), std::cref( randValues ), sec, thi );
    std::thread t4( sumUp, std::ref( sum ), std::cref( randValues ), thi, size );

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
    std::cout << "Time for addition " << dur.count() << " seconds" << std::endl;
    std::cout << "Result: " << sum << std::endl;
    return 0;
}
