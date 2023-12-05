#pragma once

#include <vector>
#include <priority_queue>
#include "timer.h"

class TimerManager
{
public:
    TimerManager() = default;
    template<typename F, typename... Args>
    Timer * addTimer( int timeout, F && f, Args &&... args )
    {
    }
    void delTimer( Timer * timer );
    uint64_t getRecentTimeout();
    void takeAllTimeout();

private:
    struct cmp
    {
        bool operator()( Timer * const & lhs, Timer * const & rhs ) const
        {
            return lhs->getExpire() > rhs->getExpire();
        }
    };
    std::priority_queue<Timer *, std::vector<Timer *>, cmp> queue_;
};
