#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>

#include "Buffer.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

using namespace std;

void oneClient(int msgs, int wait) {
    Socket* clientSocket = new Socket();
    clientSocket->Connect("127.0.0.1", "9527");
    ErrorIf(clientSocket->GetFd() == -1, "clientSocketet Connect error");

    int clientSocketfd = clientSocket->GetFd();

    Buffer* sendBuffer = new Buffer();
    Buffer* readBuffer = new Buffer();

    sleep(wait);
    int count = 0;
    while (count < msgs) {
        sendBuffer->SetBuf("I'm client!");
        ssize_t write_bytes = write(clientSocketfd, sendBuffer->ToStr(), sendBuffer->Size());
        if (write_bytes == -1) {
            printf("clientSocketet already disConnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[1024];  // 这个buf大小无所谓
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(clientSocketfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->Append(buf, read_bytes);
                already_read += read_bytes;
            } else if (read_bytes == 0) {  // EOF
                printf("server disConnected!\n");
                exit(EXIT_SUCCESS);
            }
            if ((size_t)already_read >= sendBuffer->Size()) {
                printf("count: %d, message from server: %s\n", count++, readBuffer->ToStr());
                break;
            }
        }
        readBuffer->Clear();
    }
    delete clientSocket;
}
int main(int argc, char** argv) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char* optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't':
                threads = stoi(optarg);
                break;
            case 'm':
                msgs = stoi(optarg);
                break;
            case 'w':
                wait = stoi(optarg);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }

    ThreadPool* poll = new ThreadPool(threads);
    std::function<void()> func = std::bind(oneClient, msgs, wait);
    for (int i = 0; i < threads; ++i) {
        poll->Add(func);
    }
    delete poll;
}
