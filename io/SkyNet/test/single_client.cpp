#include <unistd.h>

#include <cstring>
#include <iostream>
#include <memory>

#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "Util.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << argv[0] << " Usage<ip><port>\n";
        return 1;
    }

    std::unique_ptr<Socket> clientSocket = std::make_unique<Socket>();
    clientSocket->Connect(argv[1], argv[2]);
    ErrorIf(clientSocket->GetFd() == -1, "socket Conect error");

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(nullptr, clientSocket.get());
    while (true) {
        conn->GetlineSendBuffer();
        conn->Write();
        if (conn->GetState() == Connection::State::Closed) {
            conn->Close();
            break;
        }
        conn->Read();
        std::cout << "Message from server:" << conn->ReadBuffer() << "\n";
    }

    return 0;
}
