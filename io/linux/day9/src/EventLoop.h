#pragma once

#include "util.h"

class Epoll;
class Channel;
class ThreadPool;
class EventLoop
{

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel( Channel * ch );
    void addThread( Task task );

private:
    Epoll * ep;
    ThreadPool * threadPool;
    bool quit;
};
