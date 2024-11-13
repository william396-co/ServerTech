#pragma once

#include <memory>

#include "Common.h"

class EventLoop {
   public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE(EventLoop);

    void Loop();
    void UpdateChannel(Channel* ch);
    void DeleteChannel(Channel* ch);
    void Quit() { quit_ = true; }

   private:
    std::unique_ptr<Poller> poller_{};
    bool quit_{};
};
