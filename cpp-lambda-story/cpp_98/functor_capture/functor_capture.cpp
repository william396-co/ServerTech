#include <algorithm>
#include <iostream>
#include <vector>

struct PrintFunctor
{
    PrintFunctor()
        : numCalls {} {}

    void operator()( int x ) const
    {
        std::cout << x << "\n";
        ++numCalls;
    }
    mutable int numCalls;
};

int main()
{

    std::vector<int> v { 1, 2, 3, 5 };

    const PrintFunctor visitor = std::for_each( v.begin(), v.end(), PrintFunctor() );

    std::cout << "num Calls:" << visitor.numCalls << "\n";

    return 0;
}
