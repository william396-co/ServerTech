#pragma once

#include <string>

class Buffer
{
public:
    Buffer();
    ~Buffer();
    void append( const char * _str, size_t sz );
    ssize_t size() const { return buf.size(); }
    const char * c_str() { return buf.c_str(); }
    void clear() { buf.clear(); }
    void getline();
    void setBuf( const char * data );

private:
    std::string buf;
};
