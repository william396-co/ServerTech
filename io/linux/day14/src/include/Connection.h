#pragma once

#include <functional>
#include <memory>
#include <string>
#include "Macros.h"
#include "Types.h"

class Socket;
class EventLoop;
class Channel;
class Buffer;
class Connection
{
public:
    enum State
    {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed
    };
    Connection( EventLoop * loop, Socket * s );
    ~Connection();

    DISALLOW_COPY_AND_MOVE( Connection );

    void read();
    void write();
    void send( std::string const & msg );

    inline void setDeleteConnectionCallback( DeleteConnectionCallback const & cb )
    {
        delete_connection_callback_ = cb;
    }
    void setOnConnectCallback( ConnectCallback const & cb );
    void setOnMessageCallback( MessageCallback const & cb );

    State getState() const { return state_; }
    void close();
    void setSendBuffer( const char * data );
    Buffer * getReadBuffer() { return read_buffer_; }
    const char * readBuffer();
    Buffer * getSendBuffer() { return send_buffer_; }
    const char * sendBuffer();
    void getLineSendBuffer();
    Socket * getSocket();

    void onConnect( ConnectCallback && fn );
    void onMessage( MessageCallback && fn );

private:
    EventLoop * loop_;
    Socket * sock_;
    Channel * channel_ { nullptr };
    Buffer * read_buffer_ { nullptr };
    Buffer * send_buffer_ { nullptr };
    State state_ { State::Invalid };

    DeleteConnectionCallback delete_connection_callback_;
    ConnectCallback on_connect_callback_;
    MessageCallback on_message_callback_;

    void readNonBlocking();
    void writeNonBlocking();
    void readBlocking();
    void writeBlocking();
};
