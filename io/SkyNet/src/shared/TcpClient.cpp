#include "TcpClient.h"

#include "Common.h"
#include "Socket.h"
#include "Util.h"

TcpClient::TcpClient(const char* ip, const char* port) {
    std::unique_ptr<Socket> s = std::make_unique<Socket>();
    s->Connect(ip, port);
    ErrorIf(s->fd() == -1, "Socket Connect error");
    m_conn_ = std::make_unique<Connection>(std::move(s));
}

TcpClient::~TcpClient() {}

void TcpClient::Start() {
    if (on_run_) {
        on_run_(m_conn_.get());
    }
}
