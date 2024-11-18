#include <iostream>

#include "SkyNet.h"
#include "xtime.h"

bool m_Stop = false;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    TcpServer* server = new TcpServer(argv[1]);

    Signal::signal(SIGINT, [&] { m_Stop = true; });

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

    auto mainLoop = [&server] {
        auto current = utils::now();
        // main thread
        while (!m_Stop) {
            if (utils::now() - current > 500) {
                server->recyle();
                current = utils::now();
            }
        }

        // exit
        server->closeAccept();
        server->closeAll();
        delete server;
    };

    joining_thread mainRun(mainLoop);
    joining_thread listenRun(&TcpServer::listenRun, server);
    server->spawnWorkerThreads();
    return 0;
}
