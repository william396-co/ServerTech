#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Util.h"

Socket::Socket() : fd_{-1}, r_addr_{} {}

Socket::Socket(int fd) : fd_{fd}, r_addr_{} { ErrorIf(fd_ == -1, "socket create error"); }

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

bool Socket::Connect(const char* ip, const char* port) {
    fd_ = open_clientfd(ip, port);
    ErrorIf(fd_ == -1, "client socket connect error");
    return fd_ > 0;
}

bool Socket::Listen(const char* port) {
    fd_ = open_listenfd(port);
    ErrorIf(fd_ == -1, "socket listen error");
    return fd_ > 0;
}

bool Socket::IsNonBlocking() const { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

void Socket::SetNonBlocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

int Socket::Accept(sockaddr_in& addr) {
    socklen_t addr_len = sizeof(sockaddr_in);
    int clientfd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
    ErrorIf(clientfd == -1, "socket accept error");
    return clientfd;
}
