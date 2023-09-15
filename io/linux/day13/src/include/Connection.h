#pragma once

#include <functional>
#include <memory>
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

    inline void setDeleteConnectionCallback( DeleteConnectionCallback const & cb )
    {
        delete_connection_callback_ = cb;
    }
    void setOnConnectedCallback( ConnectedCallback const & cb );

    State getState() const { return state_; }
    void close();
    void setSendBuffer( const char * data );
    Buffer * getReadBuffer() { return read_buffer_; }
    const char * readBuffer();
    Buffer * getSendBuffer() { return send_buffer_; }
    const char * sendBuffer();
    void getLineSendBuffer();
    Socket * getSocket();

    void OnConnect( std::function<void()> fn );

private:
    EventLoop * loop_;
    Socket * sock_;
    Channel * channel_ { nullptr };
    Buffer * read_buffer_ { nullptr };
    Buffer * send_buffer_ { nullptr };
    State state_ { State::Invalid };

    DeleteConnectionCallback delete_connection_callback_;
    ConnectedCallback on_connected_callback_;
    void readNonBlocking();
    void writeNonBlocking();
    void readBlocking();
    void writeBlocking();
};
