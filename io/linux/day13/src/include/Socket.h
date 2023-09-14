#pragma once

#include <arpa/inet.h>
#include "Macros.h"

class InetAddress
{
public:
    InetAddress();
    ~InetAddress() = default;
    InetAddress( const char * ip, uint16_t port );

    DISALLOW_COPY_AND_MOVE( InetAddress );

    void setAddr( sockaddr_in addr );
    const char * getIp();
    sockaddr_in & getAddr();
    uint16_t getPort();

private:
    struct sockaddr_in addr_
    {};
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
    int accept( InetAddress & _addr );

    // Client Socket function
    void connect( InetAddress & _addr );
    void connect( const char * ip, uint16_t port );

    void setNonBlocking();
    bool isNonBlocking();

    int getFd() const { return fd; }

private:
    int fd;
};
