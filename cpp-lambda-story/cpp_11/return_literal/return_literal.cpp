#include <iostream>
#include <string>

int main()
{

#define USE_STRING

#ifndef USE_STRING
    const auto testSpeedString = []( int speed ) noexcept {
        if ( speed > 100 ) {
            return "you're a super fase";
        }
        return "you're a regular";
    };

    auto str = testSpeedString( 100 );
    //    str += " driver"; // error, no+- on const char*

#else
    const auto testSpeedString = []( int speed ) -> std::string {
        if ( speed > 100 ) {
            return "you're a super fase";
        }
        return "you're a regular";
    };

    auto str = testSpeedString( 100 );
    str += " driver"; // works fine
#endif

    std::cout << str << "\n";

    return 0;
}
