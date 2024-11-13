#pragma once

#include <functional>
#include <string>

#include "Macros.h"
#include "Types.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection;

class Connection {
   public:
    enum class State { Invalid = 1, Handshaking, Connected, Closed, Failed };

    Connection(EventLoop* loop, Socket* s);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();
    void Send(std::string const& msg);
    void Business();

    void SetDeleteConnectionCallback(ConnectionCallback const& cb) { delete_connection_callback_ = cb; }
    void SetOnConnectCallback(ConnectionMessageCallback const& cb);
    void SetOnMessageCallback(ConnectionMessageCallback const& cb);
    void Close();

    Socket* GetSocket() const { return s_; }
    State GetState() const { return state_; }

    void SetSendBuffer(const char* str);

    Buffer* GetReadBuffer() const;
    const char* ReadBuffer() const;
    Buffer* GetSendBuffer() const;

    const char* SendBuffer() const;
    void GetlineSendBuffer();

    void OnConnect(OnConnectFunction fn);
    void OnMessage(OnMessageFunction fn);
    bool IsClosed() const { return state_ == State::Closed; }

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

    ConnectionCallback delete_connection_callback_{};

    ConnectionMessageCallback on_connect_callback_{};
    ConnectionMessageCallback on_message_callback_{};

    Buffer* writeBuffer_{};
    Buffer* readBuffer_{};
};
