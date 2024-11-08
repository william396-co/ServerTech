#pragma once

#include "Macros.h"

#ifdef OS_LINUX
#include <arpa/inet.h>
#endif

class Socket {
   public:
    explicit Socket(int fd);
    Socket();
    ~Socket();

    DISALLOW_COPY_AND_MOVE(Socket);

#ifndef WIN32
    int GetFd() const { return fd_; }
#else
    SOCKET GetFd() const { return fd_; }
#endif
    void SetNonBlocking();
    bool IsNonBlocking() const;

   public:
    ////////////////////////////////////////////////
    // Only Client Use
    // //////////////////////////////////////////////
    bool Connect(const char* ip, const char* port);

   public:
    //////////////////////////////////////////////////
    // Only Server Use
    //////////////////////////////////////////////////
    bool Listen(const char* port);
    int Accept(sockaddr_in& addr);

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
