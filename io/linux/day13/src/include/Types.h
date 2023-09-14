#pragma once

#include <fcntl.h>
#include <functional>

class Socket;

using DeleteConnectionCallback = std::function<void( int )>;
using ConnectionCallback = std::function<void( Socket * )>;
using ChannelReadCallback = std::function<void()>;
using ChannelWriteCallback = std::function<void()>;

using Task = std::function<void()>;
