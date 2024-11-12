#include "Acceptor.h"

#include <iostream>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* loop, char* port) : loop_{loop} {
    listenSocket_ = new Socket();
    listenSocket_->Listen(port);
    // listenSocket_->Setnonblocking(); acceptor use blocking is better

    acceptChannel_ = new Channel(loop, listenSocket_);
    ReadCallback cb = std::bind(&Acceptor::AcceptConnection, this);
    acceptChannel_->SetReadCallback(cb);
    acceptChannel_->EnableRead();
}

Acceptor::~Acceptor() {
    delete listenSocket_;
    delete acceptChannel_;
}

void Acceptor::AcceptConnection() {
    // Accept new Socket
    sockaddr_in addr{};
    Socket* clientSocket = new Socket(listenSocket_->Accept(addr));
    clientSocket->SetRemote(addr);
    std::cout << "new client fd:" << clientSocket->GetFd() << " Ip:" << clientSocket->RemoteIp()
              << " Port:" << clientSocket->RemotePort() << "\n";
    clientSocket->SetNonBlocking();

    if (new_connection_callback_) {
        new_connection_callback_(clientSocket);
    }
}
