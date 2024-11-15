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
    void Close() const { is_open_ = false; }

   private:
    std::unique_ptr<Poller> poller_{};
    mutable bool is_open_{true};
};
