#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

int main()
{

    std::vector<int> v { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    const size_t smaller5 = std::count_if( v.begin(), v.end(), std::bind2nd( std::less<int>(), 5 ) );

    std::cout << "smaller than 5 number count is : " << smaller5 << "\n";

    return 0;
}
