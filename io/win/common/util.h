#include <WS2tcpip.h>

SOCKET open_clientfd(const char* hostname, char* port);

SOCKET open_listenfd(const char* port);