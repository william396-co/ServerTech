#pragma once

#include <fcntl.h>
#include <functional>

class Socket;
class Connection;

using ConnectedCallback = std::function<void( Connection * )>;
using DeleteConnectionCallback = std::function<void( int )>;
using ConnectionCallback = std::function<void( Socket * )>;
using ChannelReadCallback = std::function<void()>;
using ChannelWriteCallback = std::function<void()>;

using Task = std::function<void()>;
