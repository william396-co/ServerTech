#include <iostream>
#include <map>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "chat_server: Usage<Port>\n";
        return 1;
    }

    std::map<int, Connection*> clients;

    TcpServer* server = new TcpServer(argv[1]);

    server->onConnect([&](Connection* conn) {
        int fd = conn->socket()->fd();
        std::cout << "New Connection fd:" << fd << "\n";
        clients[fd] = conn;
        for (auto& it : clients) {
            it.second->Send(conn->ReadBuffer());
        }
    });

    server->onRecv([&](Connection* conn) {
        if (conn->IsClosed()) {
            conn->Close();
            return;
        }
        std::cout << "Message from client:" << conn->ReadBuffer() << "\n";

        for (auto& it : clients) {
            it.second->Send(conn->ReadBuffer());
        }
    });

    return 0;
}
