#include <iostream>
#include <boost/algorithm/string.hpp>

int main()
{

    std::string str = "Hello world!";
    boost::to_upper( str );
    std::cout << str << "\n";

    return 0;
}
