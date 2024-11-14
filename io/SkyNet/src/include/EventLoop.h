#pragma once

#include <memory>

#include "Common.h"
#include "Poller.h"

class EventLoop {
   public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE(EventLoop);

    void Loop() const;
    void UpdateChannel(Channel* ch) const;
    void DeleteChannel(Channel* ch) const;

   private:
    std::unique_ptr<Poller> poller_{};
};
