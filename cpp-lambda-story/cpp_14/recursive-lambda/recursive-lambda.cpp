#include <iostream>
#include <functional>

int factorial( int n )
{
    return n > 1 ? n * factorial( n - 1 ) : 1;
}

int main()
{
    std::cout << "factorial(5)=" << factorial( 5 ) << "\n";

#if 0
    
    auto fact = []( int n ) {
        return n > 1 ? n * fact( n - 1 ) : 1;
    };
    std::cout << "fact(5)=" << fact( 5 ) << "\n";
#endif

    // #define USE_FUNCTION

#ifdef USE_FUNCTION

    // use std::function
    const std::function<int( int )> fact = [&fact]( int n ) {
        return n > 1 ? n * fact( n - 1 ) : 1;
    };

#else
    // internal lambda and a generic parameter

    const auto fact = []( int n ) noexcept {
        const auto f_impl = []( int n, auto const & impl ) noexcept -> int {
            return n > 1 ? n * impl( n - 1, impl ) : 1;
        };
        return f_impl( n, f_impl );
    };
#endif
    std::cout << "fact(5)=" << fact( 5 ) << "\n";

    return 0;
}
