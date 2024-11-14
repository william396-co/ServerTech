#include <iostream>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    TcpServer* server = new TcpServer(argv[1]);

    auto doQuit = [&]() {
        delete server;
        std::cout << "\nServer exit!\n";
        exit(0);
    };

    Signal::signal(SIGINT, [&] { doQuit(); });

    server->onConnect([](Connection* conn) {
        std::cout << "New Connection fd:" << conn->socket()->fd() << " ip:port:[" << conn->socket()->get_addr()
                  << "]\n";
    });

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
    doQuit();
}
