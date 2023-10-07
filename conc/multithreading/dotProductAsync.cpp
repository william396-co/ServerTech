#include <iostream>
#include <future>
#include <random>
#include <vector>
#include <numeric>

using namespace std;

constexpr auto NUM = 10000000;

long long getDotProduct( vector<int> const & v, vector<int> const w )
{
    auto vSize = v.size();

    auto f1 = async( [&] {
        return inner_product( &v[0], &v[vSize / 4], &w[0], 0LL );
    } );
    auto f2 = async( [&] {} );

    auto f3 = async( [&] {} );

    auto f4 = async( [&] {} );

    return f1.get() + f1.get() + f3.get() + f4.get();
}

int main()
{

    return 0;
}
