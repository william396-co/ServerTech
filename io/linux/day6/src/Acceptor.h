#pragma once

#include <functional>
#include <memory>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
using ConnectionCallback = std::function<void( Socket * )>;

class Acceptor
{
public:
    Acceptor( EventLoop * loop, const char * ip, uint16_t port );
    ~Acceptor();
    void acceptConnection() { newConCallback( s.get() ); }
    void setNewConnectionCallback( ConnectionCallback cb )
    {
        newConCallback = cb;
    }

private:
    std::unique_ptr<EventLoop> loop;
    std::unique_ptr<Socket> s;
    std::unique_ptr<InetAddress> addr;
    std::unique_ptr<Channel> acceptChannel;
    ConnectionCallback newConCallback;
};
