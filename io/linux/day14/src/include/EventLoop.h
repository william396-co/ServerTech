#pragma once

#include "Macros.h"
#include "Types.h"

class Poller;
class Channel;
class EventLoop
{

public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE( EventLoop );

    void loop();
    void updateChannel( Channel * ch );
    void deleteChannel( Channel * ch );
    void quit();

    //    void addThread( Task task );

private:
    Pooler * poller_ { nullptr };
    bool quit { false };
};
