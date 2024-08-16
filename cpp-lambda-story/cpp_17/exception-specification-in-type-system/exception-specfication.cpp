#include <iostream>
#include <type_traits>

using TNoexceptVoidFunc = void ( * )() noexcept;
void SimpleNoexceptCall( TNoexceptVoidFunc f )
{
    f();
}

using TVoidFunc = void ( * )();
void SimpleCall( TVoidFunc f )
{
    f();
}

void fNoexcept() noexcept
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void fRegular()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template<typename Callable>
void CallWith10( Callable && fn )
{
    if constexpr ( std::is_nothrow_invocable_v<Callable, int> ) {
        std::cout << "Calling fn(10) with optimisation\n";
        fn( 10 );
    } else {
        std::cout << "Calling fn(10) normally\n";
        fn( 10 );
    }
}

int main()
{
    SimpleNoexceptCall( fNoexcept );
    SimpleNoexceptCall( []() noexcept { std::cout << __PRETTY_FUNCTION__ << "\n"; } );

    std::cout << "-----------------------------\n";

    SimpleCall( fNoexcept ); // convert to regular function
    SimpleCall( fRegular );
    SimpleCall( []() noexcept { std::cout << __PRETTY_FUNCTION__ << "\n"; } );
    SimpleCall( []() { std::cout << __PRETTY_FUNCTION__ << "\n"; } );

    {

        int x { 10 };
        const auto lam = [&x]( int y ) noexcept { x += y; };
        CallWith10( lam );

        const auto lamEx = [&x]( int y ) {
            std::cout << "lamEx with x = " << x << "\n";
            x += y;
        };

        CallWith10( lamEx );
    }

    return 0;
}
