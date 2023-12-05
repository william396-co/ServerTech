#pragma once

#include <functional>
#include <cstdint>

class Timer
{
    using Func = std::function<void( void )>;

public:
    template<typename F, typename... Args>
    Timer( uint64_t expire, F && f, Args &&... args )
        : expire_ { expire }
    {
        auto lfunc = std::bind( std::forward<F>( f ), std::forward<Args>( args )... );
        func = [lfunc] {
            ( *lfunc )();
        };
    }
    inline void active() { func(); }
    inline uint64_t getExpire() const { return expire_; }

private:
    Func func;
    unsigned long long expire_;
};
