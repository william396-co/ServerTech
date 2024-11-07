#include "Connection.h"

#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop* loop, Socket* s) : loop_{loop}, s_{s} {
    ch_ = new Channel(loop_, s->GetFd());
    ch_->UseET();
    ch_->EnableReading();

    std::function<void()> cb = std::bind(&Connection::Echo, this, s_->GetFd());
    ch_->SetReadCallback(cb);

    readBuffer_ = new Buffer();
    writeBuffer_ = new Buffer();
}

Connection::~Connection() {
    delete ch_;
    delete s_;

    delete readBuffer_;
    delete writeBuffer_;
}

void Connection::Echo(int fd) {
    constexpr auto READ_BUFFER = 1024;
    char buf[READ_BUFFER];
    while (true) {  // because use io nonblocking,reading buf until is finished
        memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            readBuffer_->Append(buf, read_bytes);
        } else if (read_bytes == -1 && errno == EINTR) {  // Client noraml interrupted, continue read
            std::cout << "continue reading\n";
            continue;
        } else if (read_bytes == -1 &&
                   (errno == EAGAIN || errno == EWOULDBLOCK)) {  // NoBlocking mode, means read finished
            std::cout << "message from client fd:" << fd << " [" << readBuffer_->ToStr() << "]\n";
            Send(fd);
            readBuffer_->Clear();
            break;
        } else if (read_bytes == 0) {  // EOF, client disconnect
            std::cout << "client fd: " << fd << " disconnected\n";
            if (deleteConnectionCallback_) {
                deleteConnectionCallback_(s_);
            }
            break;
        } else {
            std::cout << "Client reset by peer\n";
            if (deleteConnectionCallback_) {
                deleteConnectionCallback_(s_);
            }
            break;
        }
    }
}

void Connection::Send(int fd) {
    char buf[readBuffer_->Size()];
    strcpy(buf, readBuffer_->ToStr());
    int data_size = readBuffer_->Size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(fd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= bytes_write;
    }
}
