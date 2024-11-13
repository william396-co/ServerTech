#include <iostream>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    TcpServer* server = new TcpServer(argv[1]);

    Signal::signal(SIGINT, [&] {
        delete server;
        std::cout << "\nTcpServer exit!\n";
        exit(0);
    });

    server->onConnect([](Connection* conn) { std::cout << "New Connection fd:" << conn->socket()->fd() << "\n"; });

    server->onRecv([](Connection* conn) {
        if (conn->IsClosed()) {
            conn->Close();
            return;
        }

        std::cout << "Message from client[" << conn->socket()->fd() << "] :" << conn->recv_buf()->c_str() << "\n ";
        if (conn->GetState() == Connection::State::Connected) {
            conn->Send(conn->recv_buf()->c_str());
        }
    });

    server->Start();

    return 0;
}
