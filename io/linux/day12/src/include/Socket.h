#pragma once

#include "Macros.h"

class InetAddress
{
public:
    InetAddress();
    ~InetAddress() = default;
    InetAddress( const char * ip, uint16_t port );

    DISALLOW_COPY_AND_MOVE( InetAddress );

    void setInetAddr( sockaddr_in _addr, socklen_t _addr_len );

    const char * getIp() const
    {
        return inet_ntoa( addr.sin_addr );
    }
    sockaddr_in & getAddr() const
    {
        return addr;
    }
    int16_t getPort() const { return ntohs( addr.sin_port ); }

private:
    struct sockaddr_in addr;
};

class Socket
{
public:
    Socket();
    explicit Socket( int fd );
    ~Socket();

    DISALLOW_COPY_AND_MOVE( Socket );

    // Server Socket function
    void bind( InetAddress & _addr );
    void listen();
    void setnonblocking();
    int accept( InetAddress & _addr );

    // Client Socket function
    void connect( InetAddress & _addr );

    int getFd() const { return fd; }

private:
    int fd;
};
