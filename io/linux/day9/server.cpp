#include "src/EventLoop.h"
#include "src/Server.h"

constexpr auto ip = "127.0.0.1";
constexpr auto port = 9527;

int main()
{
    EventLoop * loop = new EventLoop();
    Server sever( loop, ip, port );
    loop->loop();

    delete loop;

    return 0;
}
