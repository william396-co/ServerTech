#pragma once

#include <memory>

#include "Common.h"

class Connection {
   public:
    enum class State { Invalid = 1, Connecting, Connected, Closed };

    Connection(int fd, Socket* s);
    ~Connection();

    DISALLOW_COPY_AND_MOVE(Connection);

    RC Recv();
    RC Send();
    RC Send(std::string const& msg);
    void Close();

    void set_delete_connection(ConnectionCallback&& cb) { delete_connection_callback_ = cb; }
    void set_on_connect(ConnectionMessageCallback&& cb);
    void set_on_recv(ConnectionMessageCallback const& cb);

    Socket* GetSocket() const { return s_; }
    State GetState() const { return state_; }
    bool IsClosed() const { return state_ == State::Closed; }

    void set_send_buf(const char* str);
    Buffer* recv_buf() const;
    Buffer* send_buf() const;

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
    std::unique_ptr<Channel> ch_{};
    State state_{State::Invalid};

    std::unique_ptr<Buffer> send_buf_{};
    std::unique_ptr<Buffer> recv_buf_{};

    ConnectionCallback delete_connection_{};
    ConnectionMessageCallback on_recv_{};
};
