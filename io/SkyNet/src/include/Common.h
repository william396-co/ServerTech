#pragma once

#include <functional>

class Acceptor;
class TcpServer;
class EventLoop;
class Poller;
class Acceptor;
class Connection;
class Channel;
class Socket;
class BipBuffer;
class ThreadPool;

// Macros to disable copying and moving

#define DISALLOW_COPY(cname)      \
    cname(const cname&) = delete; \
    cname& operator=(const cname&) = delete;

#define DISALLOW_MOVE(cname) \
    cname(cname&&) = delete; \
    cname& operator=(cname&&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
    DISALLOW_COPY(cname);             \
    DISALLOW_MOVE(cname);

#define ASSERT(expr, message) assert((expr) && (message))

enum RC {
    RC_UNDEFINED,
    RC_SUCCESS,
    RC_SOCKET_ERROR,
    RC_POLLER_ERROR,
    RC_CONNECTION_ERROR,
    RC_ACCEPTOR_ERROR,
    RC_UNIMPLEMENTED
};

using ConnectionMessageCallback = std::function<void(Connection*)>;
using ConnectionCallback = std::function<void(int)>;

using ReadCallback = std::function<void()>;
using WriteCallback = std::function<void()>;

using OnConnectFunction = std::function<void()>;
using OnMessageFunction = std::function<void()>;

