#include "Buffer.h"

#include <cstring>
#include <iostream>

Buffer::Buffer()
{
}

Buffer::~Buffer() {}

void Buffer::append( const char * _str, size_t sz )
{
    for ( size_t i = 0; i != sz; ++i ) {
        if ( _str[i] == '\0' ) break;
        buf.push_back( _str[i] );
    }
}

void Buffer::getline()
{
    buf.clear();
    std::getline( std::cin, buf );
}
