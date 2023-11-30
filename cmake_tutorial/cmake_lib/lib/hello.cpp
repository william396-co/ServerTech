#include "hello.h"
#include "print.h"

void say_hello( std::string const & msg )
{
    printlnEx( __FUNCTION__, "(", msg, ")" );
}
