#pragma once

#include "Common.h"

#ifdef OS_LINUX
#include <arpa/inet.h>
#endif

class Socket {
   public:
    Socket();
    ~Socket();

    DISALLOW_COPY_AND_MOVE(Socket);

#ifndef WIN32
    int fd() const { return fd_; }
    void set_fd(int fd) { fd_ = fd; }
#else
    SOCKET GetFd() const { return fd_; }
#endif
    RC SetNonBlocking();
    bool IsNonBlocking() const;
    size_t RecvBufSize() const;
    std::string get_addr() const;

   public:
    ////////////////////////////////////////////////
    // Only Client Use
    // //////////////////////////////////////////////
    RC Connect(const char* ip, const char* port);

   public:
    //////////////////////////////////////////////////
    // Only Server Use
    //////////////////////////////////////////////////
    RC Listen(const char* port);
    RC Accept(int& client_fd);

   private:
#ifdef WIN32
    SOCKET fd_;
#else
    int fd_;
#endif
};
