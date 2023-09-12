#pragma once

#include <functional>
#include <fcntl.h>

class Socket;

using DeleteConnectionCallback = std::function<void( int )>;
using ConnectionCallback = std::function<void( Socket * )>;
using ChannelReadCallback = std::function<void()>;
using ChannelWriteCallback = std::function<void()>;

using Task = std::function<void()>;

void errif( bool cond, const char * msg );
