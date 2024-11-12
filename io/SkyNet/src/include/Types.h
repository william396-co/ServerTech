#pragma once

#include <functional>

class Connection;
class Socket;

using ConnectionMessageCallback = std::function<void(Connection*)>;
using ConnectionCallback = std::function<void(Socket*)>;

using ReadCallback = std::function<void()>;
using WriteCallback = std::function<void()>;

using OnConnectFunction = std::function<void()>;
using OnMessageFunction = std::function<void()>;

