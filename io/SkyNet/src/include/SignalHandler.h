#pragma once

#include <signal.h>

#include <functional>
#include <map>

std::map<int, std::function<void()>> handlers_;

void signal_handler(int sig) { handlers_[sig](); }

struct Signal {
    static void signal(int sig, std::function<void()> const& handler) {
        handlers_[sig] = handler;
        ::signal(sig, signal_handler);
    }
};
