#pragma once

#include <string>

class Buffer
{
public:
    Buffer() {}
    ~Buffer() {}

    void append( const char * str, size_t sz );
    size_t size() const { return buf_.size(); }
    const char * c_str() const { return buf_.c_str(); }
    void clear() { buf_.clear(); }
    void getline();
    void setBuf( const char * buf );

private:
    std::string buf_;
};
