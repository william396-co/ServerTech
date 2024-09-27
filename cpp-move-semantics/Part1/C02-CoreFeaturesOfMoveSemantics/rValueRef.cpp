#include <string>
#include <iostream>

std::string returnStringByValue();

int main()
{

    std::string s { "hello" };

    std::cout << "s=" << s << "\n";

    // std::string && r1 { s };                     // ERROR
    std::string && r1 = std::move( s );
    std::cout << "r1=" << r1 << "\n";
    std::cout << "s=" << s << "\n";

    std::string && r2 { std::move( s ) }; // Ok
    std::cout << "r2=" << r2 << "\n";
    std::cout << "s=" << s << "\n";

    std::string && r3 { returnStringByValue() }; // Ok, extends lifetime of return value
    std::cout << "r3=" << r3 << "\n";

    return 0;
}

std::string returnStringByValue()
{

    return "returnStringByValue";
}
