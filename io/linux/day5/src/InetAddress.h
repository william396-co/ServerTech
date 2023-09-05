#pragma once

#include <arpa/inet.h>

struct InetAddress
{
    InetAddress();
    ~InetAddress() {}
    InetAddress( const char * ip, uint16_t port );

    struct sockaddr_in addr;
    socklen_t addr_len;
};

