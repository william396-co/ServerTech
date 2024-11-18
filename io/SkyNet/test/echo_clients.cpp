#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include "SkyNet.h"

using namespace std;

void oneClient(int msgs, int wait) {
    std::unique_ptr<TcpClient> client = std::make_unique<TcpClient>("127.0.0.1", "9527");

    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    int count{};

    client->onRun([&](Connection* conn) {
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
    });

    client->Start();
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

#ifndef USE_THREADPOOL
    std::vector<joining_thread> threadlist(threads);
    for (int i = 0; i != threads; ++i) {
        threadlist.emplace_back(oneClient, msgs, wait);
    }
#else
    ThreadPool* poll = new ThreadPool(threads);
    std::function<void()> func = std::bind(oneClient, msgs, wait);
    for (int i = 0; i < threads; ++i) {
        poll->Add(func);
    }
    delete poll;
#endif
}
