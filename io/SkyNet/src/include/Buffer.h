#pragma once

#include <string>

#include "Macros.h"

class Buffer {
   public:
    Buffer() {}
    ~Buffer() {}

    DISALLOW_COPY_AND_MOVE(Buffer);

    void Append(const char* str, size_t sz);
    size_t Size() const { return buf_.size(); }
    const char* ToStr() const { return buf_.c_str(); }
    void Clear() { buf_.clear(); }
    void Getline();
    void SetBuf(const char* buf);

   private:
    std::string buf_;
};
