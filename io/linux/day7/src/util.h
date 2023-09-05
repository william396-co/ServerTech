#pragma once

#include <functional>
#include <fcntl.h>

class Socket;

using ConnectionCallback = std::function<void( Socket * )>;
using ChannelCallback = std::function<void()>;

void errif( bool cond, const char * msg );
