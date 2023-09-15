#pragma once

#define OS_LINUX

// Macors to disable copying and moving

#define DISALLOW_COPY( cname )       \
    cname( cname const & ) = delete; \
    cname operator=( cname const & ) = delete;

#define DISALLOW_MOVE( cname )  \
    cname( cname && ) = delete; \
    cname & operator=( cname && ) = delete;

#define DISALLOW_COPY_AND_MOVE( cname ) \
    DISALLOW_COPY( cname );             \
    DISALLOW_MOVE( cname );
