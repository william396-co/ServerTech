#include <unistd.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << argv[0] << " Usage<ip><port>\n";
        return 1;
    }

    std::unique_ptr<Socket> clientSocket = std::make_unique<Socket>();
    clientSocket->Connect(argv[1], argv[2]);
    ErrorIf(clientSocket->fd() == -1, "socket Conect error");

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(std::move(clientSocket));
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        // input = "this is debug";
        conn->set_send_buf(input.c_str());
        conn->Send();
        if (conn->IsClosed()) {
            conn->Close();
            break;
        }
        conn->Recv();
        std::cout << "Message from server:" << conn->recv_buf()->c_str() << "\n";
    }

    return 0;
}
