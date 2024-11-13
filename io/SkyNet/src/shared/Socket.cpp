#include "Socket.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Util.h"

Socket::Socket() : fd_{-1} {}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

RC Socket::Connect(const char* ip, const char* port) {
    fd_ = open_clientfd(ip, port);
    if (fd_ == -1) {
        perror("Failed to connect socket");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Listen(const char* port) {
    fd_ = open_listenfd(port);
    if (fd_ == -1) {
        perror("Failed to listen socket");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

bool Socket::IsNonBlocking() const { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

RC Socket::SetNonBlocking() {
    if (fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK) == -1) {
        perror("Socket set non-blocking failed");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

RC Socket::Accept(int& client_fd) {
    struct sockaddr_in addr {};
    socklen_t addr_len = sizeof(sockaddr_in);
    client_fd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
    if (client_fd == -1) {
        perror("Failed to connect socket");
        return RC_SOCKET_ERROR;
    }
    return RC_SUCCESS;
}

size_t Socket::RecvBufSize() const {
    size_t size = 0;
    if (ioctl(fd_, FIONREAD, &size) == -1) {
        perror("Socket get rec buf size failed");
    }
    return size;
}

std::string Socket::get_addr() const {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    if (getpeername(fd_, (struct sockaddr*)&addr, &len) == -1) {
        return "";
    }
    std::string ret(inet_ntoa(addr.sin_addr));
    ret += ":";
    ret += std::to_string(htons(addr.sin_port));
    return ret;
}
