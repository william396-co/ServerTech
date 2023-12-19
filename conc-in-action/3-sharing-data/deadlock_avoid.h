#pragma once
#include <thread>
#include <mutex>

class some_big_object
{};

void swap( some_big_object & lhs, some_big_object & rhs );

class X
{
public:
    X( some_big_object const & sd )
        : some_detail { sd } {}
    friend void swap( X & lhs, X & rhs )
    {
        if ( &lsh == &rsh ) return;
        std::scoped_lock lock( lhs.m, rhs.m );
        swap( lhs.some_detail, rhs.some_detail );
    }

private:
    some_big_object some_datail;
    std::mutex m;
};
