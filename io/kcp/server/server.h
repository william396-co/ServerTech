#pragma once
#include "../src/util.h"
#include "../src/udpsocket.h"
#include "../include/ikcp.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <vector>

class Connection;
using ConnID = std::pair<std::string, uint16_t>;

namespace std {
template<>
struct hash<ConnID>
{
    size_t operator()( ConnID const & cid ) const
    {
        size_t val = 17;
        val = 31 * val + std::hash<std::string> {}( cid.first );
        val = 31 * val + std::hash<uint16_t> {}( cid.second );
        return val;
    }
};
} // namespace std

using ConnMap = std::unordered_map<ConnID, Connection *>;
using UdpSocketPtr = std::unique_ptr<UdpSocket>;
using UdpSocketList = std::vector<UdpSocketPtr>;

constexpr auto FD_PER_THREAD = 4;

extern bool is_running;

class Server
{
public:
    Server( uint16_t port, uint32_t conv );
    ~Server();

    Connection * findConn( const char * ip, uint16_t port ,UdpSocket* user);

    void accept();
    void run();
    void recv();

    void setmode( int mode );
    void show_data( bool _show ) { show = _show; }

    void setlostrate( int lostrate ) { lost_rate = lostrate / 2; }

private:
    UdpSocketPtr listen;
    ConnMap connections;
    ikcpcb * kcp;
    
    UdpSocketList fds;

    int md;
    uint16_t listen_port;
    bool show = false;
    int lost_rate = 0;
};

