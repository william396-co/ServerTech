#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "util.h"

Socket::Socket() : fd_{-1}, r_addr_{} {}

Socket::Socket(int fd) : fd_{fd}, r_addr_{} {
  errif(fd_ == -1, "socket create error");
}

Socket::~Socket() {
  if (fd_ != -1) {
    std::cout << __FUNCTION__ << " fd:" << fd_ << "\n";
    close(fd_);
    fd_ = -1;
  }
}

bool Socket::connect(const char* ip, const char* port) {
  fd_ = open_clientfd(ip, port);
  errif(fd_ == -1, "client socket connect error");
  return fd_ > 0;
}

bool Socket::listen(const char* port) {
  fd_ = open_listenfd(port);
  errif(fd_ == -1, "socket listen error");
  return fd_ > 0;
}

void Socket::setnonblocking() {
  fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(sockaddr_in& addr) {
  socklen_t addr_len = sizeof(sockaddr_in);
  int clientfd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
  errif(clientfd == -1, "socket accept error");
  return clientfd;
}
