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

    std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>(argv[1], argv[2]);
    client->onRun([](Connection* conn) {
        while (true) {
            std::string input;
            std::getline(std::cin, input);
            // input = "this is debug";
            conn->set_send_buf(input.c_str());
            conn->Send();
            if (conn->IsClosed()) {
                conn->Close();
                return;
            }
            conn->Recv();
            std::cout << "Message from server:" << conn->recv_buf()->c_str() << "\n";
        }
    });
    client->Start();
    return 0;
}
