#include "Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

Server::Server(EventLoop* loop, char* port) : mainReactor_{loop} {
  acceptor_ = new Acceptor(loop, port);
  NewConnCallback cb =
      std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor_->setNewConnectionCallback(cb);

  auto size = std::thread::hardware_concurrency();
  for (size_t i = 0; i != size; ++i) {
    subReactors_.emplace_back(new EventLoop());
  }

  for (size_t i = 0; i != size; ++i) {
    thpool_.emplace_back(std::thread(&EventLoop::loop, subReactors_[i]));
  }
}

Server::~Server() {
  delete acceptor_;
  for (size_t i = 0; i != thpool_.size(); ++i) {
    if (thpool_[i].joinable()) {
      thpool_[i].join();
    }
  }
  for (auto& it : subReactors_) {
    delete it;
  }
}

void Server::newConnection(Socket* s) {
  if (s->getFd() != -1) {
    int rand = s->getFd() % subReactors_.size();
    Connection* conn = new Connection(subReactors_[rand], s);
    DeleteConnectionCallback cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections_[s->getFd()] = conn;
  }
}

void Server::deleteConnection(Socket* s) {
  auto it = connections_.find(s->getFd());
  if (it != connections_.end()) {
    delete it->second;
    connections_.erase(it);
  }
}
