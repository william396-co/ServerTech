#include <WS2tcpip.h>

SOCKET open_clientfd(const char* hostname, const char* port);

SOCKET open_listenfd(const char* port);