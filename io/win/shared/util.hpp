#pragma once
#include <WS2tcpip.h>
#include "type.hpp"

SOCKET open_clientfd(const char* hostname, const char* port);

SOCKET open_listenfd(const char* port);
