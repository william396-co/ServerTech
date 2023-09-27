#pragma once
#include <signal.h>
#include <functional>
#include <map>

std::map<int, std::function<void()>> handles_;

void signal_handler( int sig )
{
    handlers_[sig]();
}

struct Signal
{
    void operator()( int sig, const std::function<void()> & handler )
    {
        handlers_[sig] = handler;
        ::signal( sig, signal_handler );
    }
};
