#include "Connection.h"

#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstring>

#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(int fd, EventLoop* loop) {
    socket_ = std::make_unique<Socket>();
    socket_->set_fd(fd);
    if (loop_) {
        socket_->SetNonBlocking();  // server connection socket use blocking mode
        ch_ = std::make_unique<Channel>(fd,(loop );
        ch_->EnableET();
        ch_->EnableRead();
    }

    recv_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();
    state_ = State::Connected;
}

Connection::~Connection() {}

void Connection::Close() {
    if (delete_connection_callback_) {
        delete_connection_callback_(socket_);
    }
}

void Connection::Business() {
    Read();
    on_message_callback_(this);
}

RC Connection::Recv() {
    if (state_ != State::Connected) {
        perror("Connection is not connected, cant not recv!");
        return RC_CONNECTION_ERROR;
    }
    assert(state_ == State::Connected && "Connection state is disconnected!");

    recv_buf_->Clear();
    if (socket_->IsNonBlocking()) {
        return ReadNonBlocking();
    } else {
        return ReadBlocking();
    }
}

RC Connection::Send(std::string const& msg) {
    set_send_buf(msg.c_str());
    Send();
    return RC_SUCCESS;
}

RC Connection::Send() {
    if (state_ != State::Connected) {
        perror("Connection is no connected, can not read");
        return RC_CONNECTION_ERROR;
    }
    RC rc = RC_UNDEFINED;
    if (socket_->IsNonBlocking())
        rc = WriteNonBlocking();
    else {
        rc = WriteBlocking();
    }
    send_buf_->Clear();
    return rc;
}

//////////////////////////////////////////////////////////////////////////////////////
RC Connection::WriteBlocking() {
    int fd = socket_->fd();
    ssize_t bytesocket_write = write(fd, writeBuffer_->ToStr(), writeBuffer_->Size());
    if (bytesocket_write == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
    return RC_SUCCESS;
}

RC Connection::ReadBlocking() {
    int fd = socket_->fd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytesocket_read = read(fd, buf, sizeof(buf));
    if (bytesocket_read > 0) {
        readBuffer_->Append(buf, bytesocket_read);
    } else if (bytesocket_read == 0) {
        printf("read EOF, blocking client fd:%d disconnected\n", fd);
        state_ = State::Closed;
    } else if (bytesocket_read == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////

void Connection::ReadNonBlocking() {
    int fd = socket_->GetFd();

    char buf[1024];
    while (true) {  // because use io nonblocking,reading buf until is finished
        memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            readBuffer_->Append(buf, read_bytes);
        } else if (read_bytes == -1 && errno == EINTR) {  // Client noraml interrupted, continue read
            printf("continue reading\n");
            continue;
        } else if (read_bytes == -1 &&
                   (errno == EAGAIN || errno == EWOULDBLOCK)) {  // NoBlocking mode, means read finished
            break;
        } else if (read_bytes == 0) {  // EOF, client disconnect
            printf("read EOF, client fd:%d disconnected\n", fd);
            state_ = State::Closed;
            break;
        } else {
            printf("Other error on client fd:%d\n", fd);
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking() {
    int fd = socket_->GetFd();
    char buf[writeBuffer_->Size()];
    memcpy(buf, writeBuffer_->ToStr(), writeBuffer_->Size());

    int data_size = writeBuffer_->Size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytesocket_write = write(fd, buf + data_size - data_left, data_left);
        if (bytesocket_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytesocket_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytesocket_write == -1) {
            printf("Other error on client fd:%d\n", fd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytesocket_write;
    }
}

void Connection::SetOnConnectCallback(ConnectionMessageCallback const& cb) { on_connect_callback_ = cb; }

void Connection::SetOnMessageCallback(ConnectionMessageCallback const& cb) {
    on_message_callback_ = cb;
    ReadCallback bus = std::bind(&Connection::Business, this);
    ch_->SetReadCallback(bus);
}

void Connection::SetSendBuffer(const char* str) { writeBuffer_->SetBuf(str); }

Buffer* Connection::GetReadBuffer() const { return readBuffer_; }
const char* Connection::ReadBuffer() const { return readBuffer_->ToStr(); }
Buffer* Connection::GetSendBuffer() const { return writeBuffer_; }

const char* Connection::SendBuffer() const { return writeBuffer_->ToStr(); }
void Connection::GetlineSendBuffer() { writeBuffer_->Getline(); }
