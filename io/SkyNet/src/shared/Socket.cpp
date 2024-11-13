#include "Socket.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Util.h"

Socket::Socket() : fd_{-1}, r_addr_{} {}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

RC Socket::Connect(const char* ip, const char* port) {
    if (fd_ = open_clientfd(ip, port))==-1){
            perror("Failed to connect socket");
            return RC_SOCKET_ERROR;
        }
    return RC_SUCCESS;
}

RC Socket::Listen(const char* port) {
    if (fd_ = open_listenfd(port))==-1){
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

RC Socket::Accept(sockaddr_in& addr) {
    socklen_t addr_len = sizeof(sockaddr_in);
    int clientfd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
    ErrorIf(clientfd == -1, "socket accept error");
    return clientfd;
}

size_t Socket::RecvBufSize() const {
    size_t size = 0;
    if (ioctl(fd_, FIONREAD, &size) == -1) {
        perror("Socket get rec buf size failed");
    }
    return size;
}

