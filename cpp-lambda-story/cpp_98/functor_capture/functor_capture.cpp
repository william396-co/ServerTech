#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

struct PrintFunctor
{
    PrintFunctor( std::string const & str )
        : strText { str }, numCalls {} {}

    void operator()( int x ) const
    {
        std::cout << strText << x << "\n";
        ++numCalls;
    }
    std::string strText;
    mutable int numCalls;
};

int main()
{

    std::vector<int> v { 1, 2, 3, 5 };

    const std::string introText( "Elem: " );

    const PrintFunctor visitor = std::for_each( v.begin(), v.end(), PrintFunctor( introText ) );

    std::cout << "num Calls:" << visitor.numCalls << "\n";

    return 0;
}
