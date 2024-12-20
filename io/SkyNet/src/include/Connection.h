#pragma once

#include <memory>

#include "BipBuffer.h"
#include "Channel.h"
#include "Common.h"
#include "Socket.h"

class Connection {
   public:
    enum class State { Invalid = 1, Connecting, Connected, Closed };

    Connection(int fd, EventLoop* loop, size_t sendbufsize, size_t recvbufsize);
    Connection(std::unique_ptr<Socket> socket, size_t sendbufsize, size_t recvbufsize);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    RC Recv();
    RC Send();
    RC Send(std::string const& msg);
    void Close() const;

    void set_delete_connection(ConnectionCallback const& cb) { delete_connection_ = std::move(cb); }
    void set_on_recv(ConnectionMessageCallback const& cb);

    Socket* socket() const { return socket_.get(); }
    State GetState() const { return state_; }
    bool IsClosed() const { return state_ == State::Closed; }

    void set_send_buf(const char* str);
    BipBuffer* recv_buf() const;
    BipBuffer* send_buf() const;

    void onConnect(OnConnectFunction fn);
    void onMessage(OnMessageFunction fn);

   private:
    void Business();
    RC ReadNonBlocking();
    RC WriteNonBlocking();
    RC ReadBlocking();
    RC WriteBlocking();

   private:
    std::unique_ptr<Socket> socket_{};
    std::unique_ptr<Channel> channel_{};
    State state_{State::Invalid};

    std::unique_ptr<BipBuffer> send_buf_{};
    std::unique_ptr<BipBuffer> recv_buf_{};

    ConnectionCallback delete_connection_{};
    ConnectionMessageCallback on_recv_{};
};
