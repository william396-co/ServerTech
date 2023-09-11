#pragma once

#include <arpa/inet.h>

class InetAddress
{
public:
    InetAddress();
    ~InetAddress() {}
    InetAddress( const char * ip, uint16_t port );
    void setInetAddr( sockaddr_in _addr, socklen_t _addr_len );

    sockaddr_in getAddr() const
    {
        return addr;
    }
    socklen_t getAddr_len() const
    {
        return addr_len;
    }

private:
    struct sockaddr_in addr;
    socklen_t addr_len;
};

