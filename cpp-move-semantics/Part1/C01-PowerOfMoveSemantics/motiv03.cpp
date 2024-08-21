#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> createAndInsert()
{
    std::vector<std::string> coll; // create vector of strings
    coll.reserve( 3 );
    std::string s = "data";  // create string object
    coll.push_back( s );     // insert string object
    coll.push_back( s + s ); // insert temporary string
    coll.push_back( s );     // insert string
    return coll;             // return vector of strings
}

int main()
{

    std::vector<std::string> v;

    v = createAndInsert();

    for ( std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it ) {
        std::cout << *it << "\n";
    }

    return 0;
}
