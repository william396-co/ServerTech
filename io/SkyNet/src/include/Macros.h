#pragma once
#include <cassert>

#define OS_LINUX

// Macros to disable copying and moving

#define DISALLOW_COPY(cname)      \
    cname(const cname&) = delete; \
    cname& operator=(const cname&) = delete;

#define DISALLOW_MOVE(cname) \
    cname(cname&&) = delete; \
    cname& operator=(cname&&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
    DISALLOW_COPY(cname);             \
    DISALLOW_MOVE(cname);

#define ASSERT(expr, message) assert((expr) && (message))
