#include "src/EventLoop.h"
#include "src/Server.h"
#include "src/Connection.h"
#include "src/Buffer.h"
#include "src/Socket.h"

#include <iostream>

constexpr auto ip = "127.0.0.1";
constexpr auto port = 9527;

int main()
{
    EventLoop * loop = new EventLoop();
    Server sever( loop, ip, port );

    server.onConnected( []( Connection * conn ) {
        conn->read();
        if ( conn->getState() == Connection::State::Closed ) {
            conn->close();
            return;
        }
        std::cout << "Message from client " << conn->getSocket()->getFd() << ": " << conn->readBuffer() << std::endl;
        conn->setSendBuffer( conn->readBuffer() );
        conn->write();
    } );

    loop->loop();

    delete loop;

    return 0;
}
