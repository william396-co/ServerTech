#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <atomic>

constexpr auto size = 100000000;

int main()
{

    std::vector<int> randValues;
    randValues.reserve( size );

    // random values
    std::random_device rd;
    std::mt19937 engine( rd() );
    std::uniform_int_distribution<> dist( 1, 10 );
    for ( int i = 0; i != size; ++i ) {
        randValues.emplace_back( dist( engine ) );
    }

    using namespace std::chrono;
    using namespace std::chrono_literals;

    std::cout << "Addition by operator +=\n";
    {
        auto start = steady_clock::now();

        std::atomic<unsigned long long> sum = {};
        std::cout << std::boolalpha << "sum.is_lock_free():" << sum.is_lock_free() << "\n";
        for ( auto const & n : randValues ) {
            sum += n;
        }

        duration<double> dur = steady_clock::now() - start;

        std::cout << "Time for sumition: " << dur.count() << " seconds\n";

        std::cout << "Result: " << sum << "\n";
    }

    std::cout << "\nAddition by fetch_add()\n";
    {
        auto start = steady_clock::now();

        std::atomic<unsigned long long> sum = {};
        std::cout << std::boolalpha << "sum.is_lock_free():" << sum.is_lock_free() << "\n";
        for ( auto const & n : randValues ) {
            sum.fetch_add( n );
        }

        duration<double> dur = steady_clock::now() - start;

        std::cout << "Time for sumition: " << dur.count() << " seconds\n";

        std::cout << "Result: " << sum << "\n";
    }

    std::cout << "\nAddition by fetch_add() with memory_order_relaxed\n";
    {
        auto start = steady_clock::now();

        std::atomic<unsigned long long> sum = {};
        std::cout << std::boolalpha << "sum.is_lock_free():" << sum.is_lock_free() << "\n";
        for ( auto const & n : randValues ) {
            sum.fetch_add( n, std::memory_order_relaxed );
        }

        duration<double> dur = steady_clock::now() - start;

        std::cout << "Time for sumition: " << dur.count() << " seconds\n";

        std::cout << "Result: " << sum << "\n";
    }

    return 0;
}
