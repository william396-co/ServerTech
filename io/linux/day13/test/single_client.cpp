#include <cstring>
#include <memory>
#include <unistd.h>

#include <iostream>
#include "Socket.h"
#include "Connection.h"

constexpr auto ip = "127.0.0.1";
constexpr auto port = 9527;

int main()
{

    Socket * client = new Socket();
    client->connect( ip, port );

    Connection * conn = new Connection( nullptr, client );

    while ( true ) {
        conn->getLineSendBuffer();
        conn->write();
        if ( conn->getState() == Connection::State::Closed ) {
            conn->close();
            break;
        }
        conn->read();
        std::cout << "Message from server: " << conn->readBuffer() << std::endl;
    }

    delete conn;
    return 0;
}
