#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Util.h"

Server::Server(EventLoop* Loop, char* port) : mainReactor_{Loop} {
    acceptor_ = new Acceptor(Loop, port);
    NewConnCallback cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->SetNewConnectionCallback(cb);

    auto size = std::thread::hardware_concurrency();
    for (size_t i = 0; i != size; ++i) {
        subReactors_.emplace_back(new EventLoop());
    }

    for (size_t i = 0; i != size; ++i) {
        thpool_.emplace_back(std::thread(&EventLoop::Loop, subReactors_[i]));
    }
}

Server::~Server() {
    for (size_t i = 0; i != thpool_.size(); ++i) {
        if (thpool_[i].joinable()) {
            thpool_[i].join();
        }
    }
    for (auto& it : subReactors_) {
        delete it;
    }
    delete acceptor_;
}

void Server::NewConnection(Socket* s) {
    ErrorIf(s->GetFd() == -1, "new connection error");
    if (s->GetFd() != -1) {
        int rand = s->GetFd() % subReactors_.size();
        Connection* conn = new Connection(subReactors_[rand], s);
        DeleteConnectionCallback cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
        conn->SetDeleteConnectionCallback(cb);
        conn->SetOnConnectCallback(on_connect_callback_);
        connections_[s->GetFd()] = conn;
    }
}

void Server::DeleteConnection(Socket* s) {
    auto it = connections_.find(s->GetFd());
    if (it != connections_.end()) {
        delete it->second;
        connections_.erase(it);
    }
}
