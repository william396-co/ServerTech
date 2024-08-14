#include <algorithm>
#include <functional>
#include <iostream>

int main()
{

    using std::placeholders::_1;
    const auto onePlus = std::bind( std::plus<int>(), _1, 1 );
    const auto minusOne = std::bind( std::minus<int>(), 1, _1 );
    std::cout << onePlus( 10 ) << "," << minusOne( 10 ) << "\n";

    return 0;
}
