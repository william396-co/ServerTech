#pragma once
#include <cstdint>
#include <functional>

#include "Macros.h"
#include "Types.h"

class Socket;
class EventLoop;
class Channel {
   public:
    Channel(EventLoop* loop, Socket* s);
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

    Socket* GetSocket() const { return s_; }

    int GetReadyEvents() const { return ready_events_; }
    int GetListenEvents() const { return listen_events_; }

    bool GetExist() const { return exist_; }
    void SetExist(bool in = true) { exist_ = in; }

    void UseET();

    void SetReadyEvents(int events);

    void SetReadCallback(ReadCallback const& cb) { read_callback_ = cb; }
    void SetWriteCallback(WriteCallback const& cb) { write_callback_ = cb; }

   private:
    EventLoop* loop_{};
    Socket* s_{};
    int listen_events_{};
    int ready_events_{};
    bool exist_{};
    ReadCallback read_callback_{};
    WriteCallback write_callback_{};
};
