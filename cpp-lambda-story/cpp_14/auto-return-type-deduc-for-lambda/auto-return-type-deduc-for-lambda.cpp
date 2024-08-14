#include <functional>
#include <iostream>

auto createMulLambda( int x )
{
    return [x]( int param ) noexcept { return x * param; };
}

int main()
{
    auto const lam = createMulLambda( 10 );

    std::cout << "sizeof(lam)=" << sizeof( lam ) << "\n"; // 4

    std::cout << "lam(2)=" << lam( 2 ) << "\n";

    return 0;
}
