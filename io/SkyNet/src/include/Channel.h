#pragma once
#include <cstdint>
#include <functional>

#include "Macros.h"

class EventLoop;
class Channel {
    using Callback = std::function<void()>;

   public:
    Channel(EventLoop* loop, int fd);
    ~Channel() {}

    DISALLOW_COPY_AND_MOVE(Channel);

    void HandleEvent();
    void EnableReading();
    int GetFd() const { return fd_; }

    uint32_t GetReadyEvents() const { return ready_events_; }
    uint32_t GetListenEvents() const { return listen_events_; }

    bool IsInEpoll() const { return inEpoll_; }
    void SetInEpoll(bool in = true) { inEpoll_ = in; }

    void UseET();

    void SetReadyEvents(uint32_t events) { ready_events_ = events; }

    void SetReadCallback(Callback const& cb) { readCallback_ = cb; }
    void SetWriteCallback(Callback cb) { readCallback_ = cb; }

   private:
    EventLoop* loop_{};
    int fd_{};
    uint32_t listen_events_{};
    uint32_t ready_events_{};
    bool inEpoll_{};
    Callback readCallback_{};
    Callback writeCallback_{};
};
