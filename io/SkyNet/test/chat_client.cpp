#include <iostream>

#include "Connection.h"
#include "Socket.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "chat_client: Usage<Ip><Port>\n";
        return 1;
    }

    Socket* s = new Socket();
    s->Connect(argv[1], argv[2]);

    Connection* conn = new Connection(nullptr, s);
    while (true) {
        conn->Read();
        std::cout << "Message from server:" << conn->ReadBuffer() << "\n";
    }

    delete conn;
    return 0;
}
