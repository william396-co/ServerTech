#pragma once

#include <arpa/inet.h>

class Socket
{
public:
    explicit Socket( int fd );
    Socket();
    ~Socket();
#ifndef WIN32
    int getFd() const { return fd_; }
#else
    SOCKET getFd() const { return fd_; }
#endif
    void setnonblocking();

public:
    ////////////////////////////////////////////////
    // Only Client Use
    // //////////////////////////////////////////////
    bool connect( const char * ip, const char * port );

public:
    //////////////////////////////////////////////////
    // Only Server Use
    //////////////////////////////////////////////////
    bool listen( const char * port );
    int accept( sockaddr_in & addr );

    /* remote address ip port */
    const char * remoteIp() const { return inet_ntoa( r_addr_.sin_addr ); }
    uint16_t remotePort() const { return ntohs( r_addr_.sin_port ); }
    void setRemote( sockaddr_in & addr ) { r_addr_ = addr; }

private:
#ifdef WIN32
    SOCKET fd_;
#else
    int fd_;
#endif
    sockaddr_in r_addr_; // remote address(for server record client address info)
};
