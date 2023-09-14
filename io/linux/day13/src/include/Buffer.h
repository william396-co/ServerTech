#pragma once

#include <string>
#include "Macros.h"

class Buffer
{
public:
    Buffer();
    ~Buffer();

    DISALLOW_COPY_AND_MOVE( Buffer );

    void append( const char * _str, size_t sz );
    size_t size() const { return buf.size(); }
    const char * c_str() { return buf.c_str(); }
    void clear() { buf.clear(); }
    void getline();
    void setBuf( const char * data );

private:
    std::string buf;
};
