#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> createAndInsert()
{
    std::vector<std::string> coll; // create vector of strings
    coll.reserve( 3 );             // reserve memory for 3 element
    std::string s = "data";        // create string object

    coll.push_back( s );              // insert string object
    coll.push_back( s + s );          // insert temporary string
    coll.push_back( std::move( s ) ); // insert string(we no longer need the value of s)

    return coll; // return vector of strings
}

int main()
{

    std::vector<std::string> v;

    v = createAndInsert();

    for ( auto const & it : v ) {
        std::cout << it << "\n";
    }

    return 0;
}
