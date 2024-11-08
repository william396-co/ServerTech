#pragma once

#include <functional>
#include <string>

#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection;

using DeleteConnectionCallback = std::function<void(Socket*)>;
using ConnectCallback = std::function<void(Connection*)>;

class Connection {
   public:
    enum class State { Invalid = 1, Handshaking, Connected, Closed, Failed };

    Connection(EventLoop* loop, Socket* s);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();

    void OnConnect(std::function<void()> fn);

    void SetDeleteConnectionCallback(DeleteConnectionCallback const& cb) { deleteConnectionCallback_ = cb; }
    void SetOnConnectCallback(ConnectCallback const& cb);
    void Close();

    Socket* GetSocket() const { return s_; }
    State GetState() const { return state_; }

    void SetSendBuffer(const char* str);

    Buffer* GetReadBuffer() const;
    const char* ReadBuffer() const;
    Buffer* GetSendBuffer() const;

    const char* SendBuffer() const;
    void GetlineSendBuffer();

   private:
    void ReadNonBlocking();
    void WriteNonBlocking();

    void ReadBlocking();
    void WriteBlocking();

   private:
    EventLoop* loop_{};
    Socket* s_{};
    Channel* ch_{};
    State state_{State::Invalid};

    DeleteConnectionCallback deleteConnectionCallback_{};
    ConnectCallback connectCallback_{};

    Buffer* writeBuffer_{};
    Buffer* readBuffer_{};
};
