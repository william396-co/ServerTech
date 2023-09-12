#pragma once

#include "util.h"
#include "Macros.h"

class Epoll;
class Channel;
class EventLoop
{

public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE( EventLoop );

    void loop();
    void updateChannel( Channel * ch );
    void addThread( Task task );

private:
    Epoll * ep;
    bool quit;
};
