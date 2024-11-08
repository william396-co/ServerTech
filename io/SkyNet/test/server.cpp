#include "Server.h"

#include <iostream>

#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Util.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    EventLoop* loop = new EventLoop();
    Server server(loop, argv[1]);

    server.OnConnect([](Connection* conn) {
        conn->Read();
        if (conn->GetState() == Connection::State::Closed) {
            conn->Close();
            return;
        }
        std::cout << "Message from client[" << conn->GetSocket()->GetFd() << "] :" << conn->ReadBuffer() << "\n ";
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });

    loop->Loop();

    delete loop;
    return 0;
}
