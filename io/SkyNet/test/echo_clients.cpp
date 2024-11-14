#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "Util.h"

using namespace std;

void oneClient(int msgs, int wait) {
    std::unique_ptr<Socket> clientSocket = std::make_unique<Socket>();
    clientSocket->Connect("127.0.0.1", "9527");
    ErrorIf(clientSocket->fd() == -1, "clientSocketet Connect error");

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(std::move(clientSocket));

    int count{};

    while (count < msgs) {
        if (conn->IsClosed()) {
            conn->Close();
            break;
        }
        conn->set_send_buf("I'm client!");
        conn->Send();
        conn->Recv();
        std::cout << "msg count" << count++ << ": " << conn->recv_buf()->c_str() << "\n";
    }
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
