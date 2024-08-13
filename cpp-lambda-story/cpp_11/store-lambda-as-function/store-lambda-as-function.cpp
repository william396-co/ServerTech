#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<std::function<std::string( std::string const & )>> vecFilters;

    size_t removedSpaceCounter = 0;

    const auto removeSpaces = [&removedSpaceCounter]( std::string const & str ) {
        std::string tmp;
        std::copy_if( str.begin(), str.end(), std::back_inserter( tmp ), []( char ch ) { return !isspace( ch ); } );
        removedSpaceCounter += str.length() - tmp.length();
        return tmp;
    };

    const auto makeUpperCase = []( std::string const & str ) {
        std::string tmp = str;
        std::transform( tmp.begin(), tmp.end(), tmp.begin(), []( unsigned char c ) { return std::toupper( c ); } );
        return tmp;
    };

    vecFilters.emplace_back( removeSpaces );
    vecFilters.emplace_back( []( std::string const & x ) { return x + " Amazing"; } );
    vecFilters.emplace_back( []( std::string const & x ) { return x + " Modern"; } );
    vecFilters.emplace_back( []( std::string const & x ) { return x + " C++"; } );
    vecFilters.emplace_back( []( std::string const & x ) { return x + " World"; } );
    vecFilters.emplace_back( makeUpperCase );

    const std::string str = " H e l l o   ";
    auto tmp = str;
    for ( auto const & entryFunc : vecFilters ) {
        tmp = entryFunc( tmp );
    }
    std::cout << tmp;

    std::cout << "\n removed spaces:" << removedSpaceCounter << "\n";

    return 0;
}
