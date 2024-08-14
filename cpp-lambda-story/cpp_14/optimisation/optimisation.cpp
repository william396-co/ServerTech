#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

int main()
{

    using namespace std::string_literals;
    const std::vector<std::string> vs = { "apple", "orange", "foobar", "lemon" };

    auto const prefix = "foo"s;

    auto result = std::find_if( vs.begin(), vs.end(), [&prefix]( std::string const & s ) { return s == prefix + "bar"s; } );

    if ( result != vs.end() ) {
        std::cout << prefix << "-something found!\n";
    }

    result = std::find_if( vs.begin(), vs.end(), [savedString = prefix + "bar"s]( std::string const & s ) { return s == savedString; } );
    if ( result != vs.end() ) {
        std::cout << prefix << "-something found!\n";
    }

    return 0;
}
