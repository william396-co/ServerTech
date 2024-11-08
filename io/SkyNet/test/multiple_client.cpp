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
    Socket* clientSocket = new Socket();
    clientSocket->Connect("127.0.0.1", "9527");
    ErrorIf(clientSocket->GetFd() == -1, "clientSocketet Connect error");

    std::unique_ptr<Connection> conn = std::make_unique<Connection>(nullptr, clientSocket);

    int count{};

    while (count < msgs) {
        conn->SetSendBuffer("I'm client!");
        conn->Write();
        if (conn->GetState() == Connection::State::Closed) {
            conn->Close();
            break;
        }
        conn->Read();
        std::cout << "msg count" << count++ << ": " << conn->ReadBuffer() << "\n";
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
