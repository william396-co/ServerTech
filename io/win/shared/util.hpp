#pragma once
#include <WS2tcpip.h>
#include "type.hpp"

#include <chrono>

SOCKET open_clientfd(const char* hostname, const char* port);

SOCKET open_listenfd(const char* port);
