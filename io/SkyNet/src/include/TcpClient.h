#pragma once

#include <functional>
#include <memory>

#include "Connection.h"

class TcpClient {
   public:
    TcpClient(const char* ip, const char* port);
    ~TcpClient();

    void onRun(std::function<void(Connection*)> const& fn) { on_run_ = std::move(fn); }

    void Start();

   private:
    std::unique_ptr<Connection> m_conn_;
    std::function<void(Connection*)> on_run_{};
};
