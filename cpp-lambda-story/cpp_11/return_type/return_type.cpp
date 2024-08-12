#include <type_traits>

int main()
{

    const auto baz = []( int x ) noexcept {
        if ( x < 20 )
            return x * 1.1;
        else
            return x * 2.1;
    };

    static_assert( std::is_same<double, decltype( baz( 10 ) )>::value, "has to be the same" );

    return 0;
}
