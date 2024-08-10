#include <algorithm>
#include <iostream>
#include <vector>

struct PrintFunctor
{
    void operator()( int x ) const
    {
        std::cout << x << "\n";
    }
};

int main()
{

    std::vector<int> v { 1, 2, 3, 5 };

    std::for_each( v.begin(), v.end(), PrintFunctor() );
    return 0;
}
