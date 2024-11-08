#include "Connection.h"

#include <unistd.h>

#include <cstdio>
#include <cstring>

#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop* loop, Socket* s) : loop_{loop}, s_{s} {
    if (loop_) {
        ch_ = new Channel(loop_, s->GetFd());
        ch_->UseET();
        ch_->EnableReading();
    }

    readBuffer_ = new Buffer();
    writeBuffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection() {
    if (loop_) {
        delete ch_;
    }
    delete s_;

    delete readBuffer_;
    delete writeBuffer_;
}

void Connection::Close() {
    if (deleteConnectionCallback_) {
        deleteConnectionCallback_(s_);
    }
}

void Connection::Read() {
    ASSERT(state_ == State::Connected, "Connection state is disconnected!");

    readBuffer_->Clear();
    if (s_->IsNonBlocking()) {
        ReadNonBlocking();
    } else {
        ReadBlocking();
    }
}

void Connection::Write() {
    ASSERT(state_ == State::Connected, "connecion state is disconnected!");
    if (s_->IsNonBlocking())
        WriteNonBlocking();
    else {
        WriteBlocking();
    }
    writeBuffer_->Clear();
}

//////////////////////////////////////////////////////////////////////////////////////
void Connection::WriteBlocking() {
    int fd = s_->GetFd();
    ssize_t bytes_write = write(fd, writeBuffer_->ToStr(), writeBuffer_->Size());
    if (bytes_write == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
}

void Connection::ReadBlocking() {
    int fd = s_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(fd, buf, sizeof(buf));
    if (bytes_read > 0) {
        readBuffer_->Append(buf, bytes_read);
    } else if (bytes_read == 0) {
        printf("read EOF, blocking client fd:%d disconnected\n", fd);
        state_ = State::Closed;
    } else if (bytes_read == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////

void Connection::ReadNonBlocking() {
    int fd = s_->GetFd();

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
    int fd = s_->GetFd();
    char buf[writeBuffer_->Size()];
    memcpy(buf, writeBuffer_->ToStr(), writeBuffer_->Size());

    int data_size = writeBuffer_->Size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(fd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd:%d\n", fd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::SetOnConnectCallback(ConnectCallback const& cb) {
    connectCallback_ = cb;
    if (ch_) {
        ch_->SetReadCallback([this]() { connectCallback_(this); });
    }
}

void Connection::SetSendBuffer(const char* str) { writeBuffer_->SetBuf(str); }

Buffer* Connection::GetReadBuffer() const { return readBuffer_; }
const char* Connection::ReadBuffer() const { return readBuffer_->ToStr(); }
Buffer* Connection::GetSendBuffer() const { return writeBuffer_; }

const char* Connection::SendBuffer() const { return writeBuffer_->ToStr(); }
void Connection::GetlineSendBuffer() { writeBuffer_->Getline(); }
