#pragma once
#include <functional>

#include "Common.h"

class Channel {
   public:
    Channel(int fd, EventLoop* loop);
    ~Channel();

    enum {
        READ_EVENT = 1,
        WRITE_EVENT = 2,
        ET = 4,
    };

    DISALLOW_COPY_AND_MOVE(Channel);

    void HandleEvent();
    void EnableRead();
    void EnableWrite();

    int fd() const { return fd_; }
    short ready_events() const { return ready_events_; }
    short listen_events() const { return listen_events_; }
    bool exist() const { return exist_; }
    void set_exist(bool in = true) { exist_ = in; }
    void EnableET();

    void set_ready_event(short events);
    void set_read_callback(ReadCallback&& cb) { read_callback_ = std::move(cb); }
    void set_write_callback(WriteCallback&& cb) { write_callback_ = std::move(cb); }

   private:
    EventLoop* loop_{};
    int fd_{};
    short listen_events_{};
    short ready_events_{};
    bool exist_{};
    ReadCallback read_callback_{};
    WriteCallback write_callback_{};
};
