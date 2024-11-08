#pragma once
#include <functional>

#include "Macros.h"

class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
   public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE(EventLoop);

    void Loop();
    void UpdateChannel(Channel* ch);

   private:
    Epoll* ep_{};
    bool quit_{};
};
