#include "Server.h"

#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#include <unistd.h>
#include <cassert>
#include <cstring>
#include <functional>
#include <thread>

constexpr auto READ_BUFFER = 1024;

Server::Server(EventLoop *_loop, const char *ip, uint16_t port) : mainReactor{_loop}, acceptor{nullptr} {
  acceptor = new Acceptor{mainReactor, ip, port};
  ConnectionCallback cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor->setNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();
  thpool = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    subReactors.emplace_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
    thpool->add(sub_loop);
  }
}

Server::~Server() {
  for (auto &it : connections) {
    delete it.second;
  }

  delete acceptor;
  delete thpool;

  for (auto &it : subReactors) {
    delete it;
  }
}

void Server::deleteConnection(int fd) {
  auto it = connections.find(fd);
  if (it != connections.end()) {
    delete it->second;
    connections.erase(it);
  }
}

void Server::newConnection(Socket *_s) {
  if (_s->getFd() != -1) {
    int random = _s->getFd() % subReactors.size();
    Connection *conn = new Connection(subReactors[random], _s);
    DeleteConnectionCallback cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections.emplace(_s->getFd(), conn);
  }
}
