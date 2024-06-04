#include <iostream>
#include <functional>
#include <cmath>

#ifndef BLOB

template<class X, class UnaryOp1, class UnaryOp2>
X apply_fg( X x, UnaryOp1 f, UnaryOp2 g )
{
    return f( g( x ) );
}
#else

template<class X, class Blob>
X apply_fg( X x, Blob blob )
{
    return blob.f( blob.g( x ) );
}
#endif

int main()
{
    int a = apply_fg( 5.0f, std::negate<float>(), log2 );
    int b = apply_fg( 3.14f, log2, std::negate<float>() );

    std::cout << "a=" << a << " b=" << b << "\n";

    return 0;
}
