#include "Acceptor.h"

#include <cassert>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* loop, const char* port) {
    socket_ = std::make_unique<Socket>();
    assert(socket_->Listen(port) == RC_SUCCESS);

    channel_ = std::make_unique<Channel>(socket_->fd(), loop);
    ReadCallback cb = std::bind(&Acceptor::AcceptConnection, this);

    channel_->set_read_callback(std::move(cb));
    channel_->EnableRead();
}

Acceptor::~Acceptor() {}

RC Acceptor::AcceptConnection() const {
    int client_fd = -1;
    if (socket_->Accept(client_fd) != RC_SUCCESS) {
        return RC_ACCEPTOR_ERROR;
    }

    if (new_connection_callback_) {
        new_connection_callback_(client_fd);
    }
    return RC_SUCCESS;
}
