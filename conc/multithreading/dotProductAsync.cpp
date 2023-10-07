#include <iostream>
#include <future>
#include <random>
#include <vector>
#include <numeric>

constexpr auto NUM = 10000000;

long long getDotProduct( std::vector<int> const & v, std::vector<int> const w )
{
    auto vSize = v.size();

    auto f1 = std::async( [&] {
        return std::inner_product( &v[0], &v[vSize / 4], &w[0], 0LL );
    } );
    auto f2 = std::async( [&] {
        return std::inner_product( &v[vSize / 4], &v[vSize / 2], &w[vSize / 4], 0LL );
    } );

    auto f3 = std::async( [&] {
        return std::inner_product( &v[vSize / 2], &v[vSize * 3 / 4], &w[vSize / 2], 0LL );
    } );

    auto f4 = std::async( [&] {
        return std::inner_product( &v[vSize * 3 / 4], &v[vSize], &w[vSize * 3 / 4], 0LL );
    } );

    return f1.get() + f1.get() + f3.get() + f4.get();
}

int main()
{

    std::random_device rd;
    // generator
    std::mt19937 engine( rd() );

    // distribution
    std::uniform_int_distribution<int> dist( 0, 100 );

    std::vector<int> v, w;
    v.reserve( NUM );
    w.reserve( NUM );
    for ( int i = 0; i < NUM; ++i ) {
        v[i] = dist( engine );
        w[i] = dist( engine );
    }

    std::cout << "getDotProduct(v,w): " << getDotProduct( v, w ) << "\n";

    return 0;
}
