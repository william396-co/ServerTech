#include "Acceptor.h"

#include <cassert>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* loop, char* port) {
    listenSocket_ = std::make_unique<Socket>();
    assert(listenSocket_->Listen(port) == RC_SUCCESS);

    acceptChannel_ = std::make_unique<Channel>(listenSocket_->fd(), loop);

    ReadCallback cb = std::bind(&Acceptor::AcceptConnection, this);
    acceptChannel_->set_read_callback(cb);

    acceptChannel_->EnableRead();
}

Acceptor::~Acceptor() {}

RC Acceptor::AcceptConnection() const {
    int fd = -1;
    if (listenSocket_->Accept(fd) != RC_SUCCESS) {
        return RC_ACCEPTOR_ERROR;
    }

    if (new_connection_callback_) {
        new_connection_callback_(clientSocket);
    }
    return RC_SUCCESS;
}
