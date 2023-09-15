#pragma once

#include <fcntl.h>
#include <functional>

class Socket;
using NewConnectionCallback = std::function<void( Socket * )>;

using DeleteConnectionCallback = std::function<void( int )>;

class Connection;
using ConnectedCallback = std::function<void( Connection * )>;

using ChannelReadCallback = std::function<void()>;
using ChannelWriteCallback = std::function<void()>;

using Task = std::function<void()>;
