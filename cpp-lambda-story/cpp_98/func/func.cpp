#include <algorithm>
#include <iostream>
#include <vector>

void PrintFunc( int x )
{
    std::cout << x << "\n";
}

int main()
{

    std::vector<int> v { 1, 2, 3, 5 };

    std::for_each( v.begin(), v.end(), PrintFunc );
    return 0;
}
