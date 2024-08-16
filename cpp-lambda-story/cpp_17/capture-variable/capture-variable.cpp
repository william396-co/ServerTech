#include <iostream>
#include <cassert>

constexpr int add( int const & t, int const & u )
{
    return t + u;
}

int main()
{

    constexpr int x = 0;
    constexpr auto lam = [x]( int n ) { return add( x, n ); };

    static_assert( lam( 10 ) == 10 );

    return 0;
}
