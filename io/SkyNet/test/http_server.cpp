#include <iostream>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop, argv[1]);

    Signal::signal(SIGINT, [&] {
        delete server;
        delete loop;
        std::cout << "\nServer exit!\n";
        exit(0);
    });

    server->OnMessage([](Connection* conn) {
        if (conn->IsClosed()) {
            conn->Close();
            return;
        }
        std::cout << "Message from client[" << conn->GetSocket()->GetFd() << "] :" << conn->ReadBuffer() << "\n ";
        if (conn->GetState() == Connection::State::Connected) {
            conn->Send(conn->ReadBuffer());
        }
    });

    loop->Loop();
    return 0;
}
