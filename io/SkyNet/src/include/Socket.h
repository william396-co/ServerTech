#pragma once

#include "Macros.h"

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
    RC Accept(sockaddr_in& addr);

    /* remote address ip port */
    const char* RemoteIp() const { return inet_ntoa(r_addr_.sin_addr); }
    uint16_t RemotePort() const { return ntohs(r_addr_.sin_port); }
    void SetRemote(sockaddr_in& addr) { r_addr_ = addr; }

   private:
#ifdef WIN32
    SOCKET fd_;
#else
    int fd_;
#endif
    // remote address(for server record client address info)
    sockaddr_in r_addr_;
};
