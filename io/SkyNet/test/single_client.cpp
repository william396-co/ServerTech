#include <unistd.h>

#include <cstring>
#include <iostream>
#include <memory>

#include "Buffer.h"
#include "Socket.h"
#include "util.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << argv[0] << " Usage<ip><port>\n";
        return 1;
    }

    std::unique_ptr<Socket> clientSocket = std::make_unique<Socket>();
    clientSocket->Connect(argv[1], argv[2]);
    ErrorIf(clientSocket->GetFd() == -1, "socket Conect error");

    std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();
    std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();
    auto clientfd = clientSocket->GetFd();

    while (true) {
        sendBuffer->Getline();
        ssize_t write_bytes = write(clientfd, sendBuffer->ToStr(), sendBuffer->Size());
        if (write_bytes == -1) {
            std::cout << "socket already disConected,can't write any more!\n";
            break;
        }

        size_t already_read = 0;
        char buf[1024];
        while (true) {
            ssize_t read_bytes = read(clientfd, buf, sizeof(buf));
            if (read_bytes) {
                readBuffer->Append(buf, read_bytes);
                already_read += read_bytes;
                //                std::cout << "message from server:" << buf <<
                //                "\n";
            } else if (read_bytes == 0) {
                std::cout << "server socketd disConected!\n";
                exit(EXIT_FAILURE);
            } else if (read_bytes == -1) {
                ErrorIf(true, "socket read error");
            }

            if (already_read >= sendBuffer->Size()) {
                std::cout << "message from server: [" << readBuffer->ToStr() << "]\n";
                break;
            }
        }
        readBuffer->Clear();
    }

    return 0;
}
