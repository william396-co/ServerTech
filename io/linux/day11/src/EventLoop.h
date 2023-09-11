#pragma once

#include "util.h"

class Epoll;
class Channel;
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
    bool quit;
};
