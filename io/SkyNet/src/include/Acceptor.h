#pragma once
#include <functional>
#include <memory>

#include "Common.h"

class Acceptor {
   public:
    Acceptor(EventLoop* loop, const char* port);
    ~Acceptor();

    DISALLOW_COPY_AND_MOVE(Acceptor);

    RC AcceptConnection() const;
    void set_new_connection_callback(ConnectionCallback const& cb) { new_connection_callback_ = std::move(cb); }

   private:
    std::unique_ptr<Socket> socket_{};
    std::unique_ptr<Channel> channel_{};
    ConnectionCallback new_connection_callback_{};
};
