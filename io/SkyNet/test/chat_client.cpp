#include <iostream>
#include <memory>
#include <string>

#include "Connection.h"
#include "Socket.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "chat_client: Usage<Ip><Port>\n";
        return 1;
    }

    std::unique_ptr<Socket> s = std::make_unique<Socket>();
    s->Connect(argv[1], argv[2]);

    Connection* conn = new Connection(std::move(s));
    while (true) {
        if (conn->IsClosed()) {
            conn->Close();
            break;
        }

        conn->Send("Hello Server");

        conn->Recv();
        std::cout << "Message from server:" << conn->recv_buf()->c_str() << "\n";
    }

    delete conn;
    return 0;
}
