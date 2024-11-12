#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Exception.h"
#include "Socket.h"
#include "Util.h"
#ifdef USE_THREADPOOL
#include "ThreadPool.h"
#endif

Server::Server(EventLoop* loop, char* port) : mainReactor_{loop} {
    if (!mainReactor_) {
        throw Exception(ExceptionType::INVALID, "main reactor can't be nullptr");
    }

    acceptor_ = new Acceptor(mainReactor_, port);
    ConnectionCallback cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->SetNewConnectionCallback(cb);

    auto size = std::thread::hardware_concurrency();
    for (size_t i = 0; i != size; ++i) {
        subReactors_.emplace_back(new EventLoop());
    }

#ifndef USE_THREADPOOL
    for (size_t i = 0; i != size; ++i) {
        thpool_.emplace_back(std::thread(&EventLoop::Loop, subReactors_[i]));
    }
#else
    thread_pool_ = new ThreadPool(size);
    for (size_t i = 0; i != size; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, subReactors_[i]);
        thread_pool_->Add(std::move(sub_loop));
    }

#endif
}

Server::~Server() {
#ifndef USE_THREADPOOL
    for (size_t i = 0; i != thpool_.size(); ++i) {
        if (thpool_[i].joinable()) {
            thpool_[i].join();
        }
    }
#else
    delete thread_pool_;
#endif
    for (auto& it : subReactors_) {
        delete it;
    }
    delete acceptor_;
}

void Server::NewConnection(Socket* s) {
    //  ErrorIf(s->GetFd() == -1, "new connection error");
    if (s->GetFd() == -1) {
        throw Exception(ExceptionType::INVALID_SOCKET, "New Connection error, invalid client socket!");
    }
    if (s->GetFd() != -1) {
        int rand = s->GetFd() % subReactors_.size();
        Connection* conn = new Connection(subReactors_[rand], s);
        ConnectionCallback cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
        conn->SetDeleteConnectionCallback(cb);
        conn->SetOnMessageCallback(on_message_callback_);
        connections_[s->GetFd()] = conn;
        if (new_connect_callback_) {
            new_connect_callback_(conn);
        }
    }
}

void Server::DeleteConnection(Socket* s) {
    auto it = connections_.find(s->GetFd());
    if (it != connections_.end()) {
        delete it->second;
        connections_.erase(it);
    }
}
