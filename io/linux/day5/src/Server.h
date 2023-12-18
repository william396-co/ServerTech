#pragma once

#include <unordered_map>
#include <cstdint>

class EventLoop;
class Socket;
class Channel;

using SocketList = std::unordered_map<int, Socket *>;
using ChannelList = std::unordered_map<int, Channel *>;

class Server
{
public:
    Server( EventLoop * loop, const char * ip, uint16_t port );
    ~Server() { clear(); }

    void handleReadEvent( int fd );
    void newConnection( Socket * listenfd );

private:
    void addSocketChannel( int fd, Socket * s, Channel * ch );
    void delSocketChannel( int fd );
    void clear();

private:
    EventLoop * loop;
    SocketList socketList;
    ChannelList channelList;
};
