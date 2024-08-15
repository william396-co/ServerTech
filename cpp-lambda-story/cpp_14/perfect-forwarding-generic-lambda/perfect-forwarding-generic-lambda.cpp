#include <iostream>
#include <string>

void foo( std::string const & str )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}
void foo( std::string && str )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template<typename T>
void callFooFunc( T && str )
{
    std::cout << "Calling" << __PRETTY_FUNCTION__ << " on:" << str << "\n";
    foo( std::forward<T>( str ) );
}

int main()
{

    const auto callFoo = []( auto && str ) {
        std::cout << "Calling foo() on:" << str << "\n";
        foo( std::forward<decltype( str )>( str ) );
    };

    const std::string str = "Hello World";
    callFoo( str );
    callFoo( "Hello World Ref Ref" );

    callFooFunc( str );
    callFooFunc( "helo world ref ref" );

    return 0;
}

