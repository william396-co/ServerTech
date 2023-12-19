#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <mutex>

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

    //  auto start = duration_cast<milliseconds>( steady_clock::now().time_since_epoch() ).count();
    auto start = steady_clock::now();

    std::mutex mtx;
    unsigned long long sum = {};
    for ( auto const & n : randValues ) {
        std::lock_guard lock( mtx );
        sum += n;
    }

    //    auto dur = duration_cast<milliseconds>( steady_clock::now().time_since_epoch() ).count() - start;
    duration<double> dur = steady_clock::now() - start;

    std::cout << "Time for sumition: " << dur.count() << " seconds\n";

    std::cout << "Result: " << sum << "\n";

    return 0;
}
