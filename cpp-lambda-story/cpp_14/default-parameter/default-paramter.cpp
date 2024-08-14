#include <iostream>

int main()
{

    auto const lam = []( int x = 10 ) { std::cout << x << "\n"; };

    lam();
    lam( 100 );

    return 0;
}
