#include <algorithm>
#include <map>
#include <string>
#include <iostream>

int main()
{

    const std::map<std::string, int> numbers {
        { "one", 1 }, { "two", 2 }, { "three", 3 }
    };

    // print address
    for ( auto mit = numbers.cbegin(); mit != numbers.cend(); ++mit ) {
        std::cout << &mit->first << "," << &mit->second << "\n";
    }

    // each time entry is copied from pair<const string, int>
    std::for_each( std::begin( numbers ), std::end( numbers ), []( std::pair<std::string, int> const & entry ) {
        std::cout << &entry.first << "," << &entry.second << ":"
                  << entry.first << " = " << entry.second << "\n";
    } );

    // this time entries are not copied, they have the same addresses
    std::for_each( std::begin( numbers ), std::end( numbers ), []( auto const & entry ) {
        std::cout << &entry.first << "," << &entry.second << ":"
                  << entry.first << " = " << entry.second << "\n";
    } );
    return 0;
}
