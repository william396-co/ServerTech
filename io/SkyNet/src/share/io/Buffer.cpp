#include "Buffer.h"

#include <iostream>

void Buffer::append( const char * str, size_t sz )
{
    for ( auto i = 0; i != sz; ++i ) {
        if ( str[i] == '\0' ) break;

        buf_.push_back( str[i] );
    }
}

void Buffer::getline()
{
    buf_.clear();
    std::getline( std::cin, buf_ );
}
