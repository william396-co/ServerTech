#pragma once

struct InetAddress;
class Socket
{
public:
    Socket();
    Socket( int fd );
    ~Socket();

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
