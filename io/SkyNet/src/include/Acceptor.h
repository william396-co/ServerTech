#pragma once
#include <functional>
#include <memory>

#include "Common.h"

class Acceptor {
   public:
    explicit Acceptor(char* port);
    ~Acceptor();

    DISALLOW_COPY_AND_MOVE(Acceptor);

    RC AcceptConnection() const;
    void set_new_connection_callback(ConnectionCallback&& cb) { new_connection_callback_ = std::move(cb); }

   private:
    std::unique_ptr<Socket> listenSocket_{};
    std::unique_ptr<Channel> acceptChannel_{};
    ConnectionCallback new_connection_callback_{};
};
