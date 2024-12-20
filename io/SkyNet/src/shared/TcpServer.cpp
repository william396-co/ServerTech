#include "TcpServer.h"

#include <unistd.h>

#include <cassert>
#include <cstring>
#include <functional>

#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Exception.h"
#include "Socket.h"
#include "Util.h"
#ifdef USE_THREADPOOL
#include "ThreadPool.h"
#endif

TcpServer::TcpServer(const char* port) {
    main_reactor_ = std::make_unique<EventLoop>();

    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get(), port);
    ConnectionCallback cb = std::bind(&TcpServer::NewConnection, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(std::move(cb));

    size_t size = std::thread::hardware_concurrency();
    for (size_t i = 0; i != size; ++i) {
        std::unique_ptr<EventLoop> sub_loop = std::make_unique<EventLoop>();
        sub_reactors_.emplace_back(std::move(sub_loop));
    }
#ifndef USE_THREADPOOL
#else
    thread_pool_ = std::make_unique<ThreadPool>(size);
#endif
}

TcpServer::~TcpServer() {}

void TcpServer::spawnWorkerThreads() {
#ifndef USE_THREADPOOL
    for (size_t i = 0; i != sub_reactors_.size(); ++i) {
        thpool_.emplace_back(&EventLoop::Loop, sub_reactors_[i].get());
    }
#else
    for (size_t i = 0; i != sub_reactors_.size(); ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i].get());
        thread_pool_->Add(std::move(sub_loop));
    }
#endif
}

void TcpServer::closeAll() const {
    for (size_t i = 0; i != sub_reactors_.size(); ++i) {
        sub_reactors_[i]->Close();
    }
    for (auto const& it : connections_) {
        it.second->Close();
    }
}
void TcpServer::closeAccept() const {
    acceptor_->Close();
    main_reactor_->Close();
}

void TcpServer::listenRun() { main_reactor_->Loop(); }

void TcpServer::recyle() {
    for (auto it = connections_.begin(); it != connections_.end();) {
        if (it->second->IsClosed()) {
            it = connections_.erase(it);
        } else {
            ++it;
        }
    }
}

RC TcpServer::NewConnection(int fd) {
    assert(fd != -1);
    int rand = fd % sub_reactors_.size();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(fd, sub_reactors_[rand].get());
    ConnectionCallback cb = std::bind(&TcpServer::DeleteConnection, this, std::placeholders::_1);

    conn->set_delete_connection(cb);
    conn->set_on_recv(on_recv_);
    connections_[fd] = std::move(conn);
    if (on_connect_) {
        on_connect_(connections_[fd].get());
    }
    return RC_SUCCESS;
}

RC TcpServer::DeleteConnection(int fd) {
    auto it = connections_.find(fd);
    if (it != connections_.end()) {
        connections_.erase(it);
    }
    return RC_SUCCESS;
}
