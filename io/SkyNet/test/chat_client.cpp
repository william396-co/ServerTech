#include <iostream>
#include <memory>
#include <string>

#include "SkyNet.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "chat_client: Usage<Ip><Port>\n";
        return 1;
    }

    std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(argv[1], argv[2]);

    client->onRun([](Connection* conn) {
        while (true) {
            if (conn->IsClosed()) {
                conn->Close();
                break;
            }

            conn->Send("Hello Server");

            conn->Recv();
            std::cout << "Message from server:" << conn->recv_buf()->c_str() << "\n";
        }
    });
    return 0;
}
