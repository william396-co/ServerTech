#pragma once

#include <string>

#include "Common.h"

class Buffer {
   public:
    Buffer() {}
    ~Buffer() {}

    DISALLOW_COPY_AND_MOVE(Buffer);
    void Clear() { buf_.clear(); }
    void Append(const char* str, size_t sz);
    size_t Size() const { return buf_.size(); }

    const char* c_str() const { return buf_.c_str(); }
    const std::string& buf() const { return buf_; }
    void set_buf(const char* buf);

   private:
    std::string buf_;
};
