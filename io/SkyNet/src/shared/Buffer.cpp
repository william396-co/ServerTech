#include "Buffer.h"

#include <iostream>

void Buffer::Append(const char* str, size_t sz) {
    for (size_t i = 0; i != sz; ++i) {
        if (str[i] == '\0') break;

        buf_.push_back(str[i]);
    }
}

void Buffer::Getline() {
    buf_.clear();
    std::getline(std::cin, buf_);
}

void Buffer::SetBuf(const char* buf) {
    buf_.clear();
    buf_.append(buf);
}
