#pragma once

struct InetAddress;
class Socket
{
public:
    Socket();
    Socket( int fd );
    ~Socket();

    void bind( InetAddress & addr );
    void listen();
    void setnonblocking();
    int accept( InetAddress & addr );
    int getFd() const { return fd; }

private:
    int fd;
};
