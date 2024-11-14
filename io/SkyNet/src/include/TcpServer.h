#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Common.h"
#ifdef USE_THREADPOOL
#include "ThreadPool.h"
#endif

class TcpServer {
    using ConnectionMap = std::unordered_map<int, std::unique_ptr<Connection>>;
    using ReactorList = std::vector<std::unique_ptr<EventLoop>>;
    using ThreadList = std::vector<std::thread>;

   public:
    explicit TcpServer(const char* port);
    ~TcpServer();
    void Start();

    DISALLOW_COPY_AND_MOVE(TcpServer);

    RC NewConnection(int fd);
    RC DeleteConnection(int fd);

    void onConnect(ConnectionMessageCallback const& fn) { on_connect_ = std::move(fn); }
    void onRecv(ConnectionMessageCallback const& fn) { on_recv_ = std::move(fn); }

   private:
    std::unique_ptr<EventLoop> main_reactor_{};
    std::unique_ptr<Acceptor> acceptor_{};
    ReactorList sub_reactors_{};
#ifndef USE_THREADPOOL
    ThreadList thpool_;
#else
    std::unique_ptr<ThreadPool> thread_pool_{};
#endif

    ConnectionMap connections_;

    ConnectionMessageCallback on_connect_{};
    ConnectionMessageCallback on_recv_{};
};
