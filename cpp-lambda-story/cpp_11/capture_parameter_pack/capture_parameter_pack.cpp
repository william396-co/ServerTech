#include <iostream>
#include <tuple>

template<typename... Args>
void captureTest( Args &&... args )
{
    const auto lambda = [args...] {
        const auto tup = std::make_tuple( args... );
        std::cout << "tuple size: " << std::tuple_size<decltype( tup )>::value << "\n";

        std::cout << "tuple 1st: " << std::get<0>( tup ) << "\n";
    };

    lambda();
}

int main()
{
    captureTest( 1, 2, 3, 4 );

    captureTest( "hello world", 10.0f );

    return 0;
}
