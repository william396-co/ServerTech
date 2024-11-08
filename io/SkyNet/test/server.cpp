#include "Server.h"

#include <iostream>

#include "EventLoop.h"
#include "util.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << argv[0] << " Usage<port>\n";
        return 1;
    }

    EventLoop* Loop = new EventLoop();
    Server server(Loop, argv[1]);
    Loop->Loop();

    delete Loop;
    return 0;
}
