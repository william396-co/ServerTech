#include <iostream>
#include <map>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "chat_server: Usage<Port>\n";
        return 1;
    }

    std::map<int, Connection*> clients;

    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop, argv[1]);

    server->NewConnect([&](Connection* conn) {
        int fd = conn->GetSocket()->GetFd();
        std::cout << "New Connection fd:" << fd << "\n";
        clients[fd] = conn;
        for (auto& it : clients) {
            it.second->Send(conn->ReadBuffer());
        }
    });

    server->OnMessage([&](Connection* conn) {
        if (conn->IsClosed()) {
            conn->Close();
            return;
        }
        std::cout << "Message from client:" << conn->ReadBuffer() << "\n";

        for (auto& it : clients) {
            it.second->Send(conn->ReadBuffer());
        }
    });

    loop->Loop();

    return 0;
}
