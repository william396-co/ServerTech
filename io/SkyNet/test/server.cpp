#include "Server.h"

#include <iostream>

#include "EventLoop.h"
#include "util.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    EventLoop* loop = new EventLoop();
    Server server(loop, argv[1]);
    loop->loop();

    delete loop;
    return 0;
}
