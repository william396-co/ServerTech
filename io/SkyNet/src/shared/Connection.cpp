#include "Connection.h"

#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstring>

#include "EventLoop.h"

Connection::Connection(int fd, EventLoop* loop) {
    socket_ = std::make_unique<Socket>();
    socket_->set_fd(fd);
    socket_->SetNonBlocking();
    if (loop) {
        channel_ = std::make_unique<Channel>(fd, loop);
        channel_->EnableET();
        channel_->EnableRead();
    }

    recv_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();
    state_ = State::Connected;
}

Connection::Connection(std::unique_ptr<Socket> socket) : socket_{std::move(socket)} {
    recv_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();
    state_ = State::Connected;
}

Connection::~Connection() {}

void Connection::Close() {
    if (delete_connection_) {
        delete_connection_(socket_->fd());
    }
}

void Connection::Business() {
    Recv();
    on_recv_(this);
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

RC Connection::Send() {
    if (state_ != State::Connected) {
        perror("Connection is not connected, can not send");
        return RC_CONNECTION_ERROR;
    }
    RC rc = RC_UNDEFINED;
    if (socket_->IsNonBlocking()) {
        rc = WriteNonBlocking();
    } else {
        rc = WriteBlocking();
    }
    return rc;
}

RC Connection::Send(std::string const& msg) {
    set_send_buf(msg.c_str());
    Send();
    return RC_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////
RC Connection::WriteBlocking() {
    // TODO didn't handle about tcp sendbuffer overflow tcp buffer
    int fd = socket_->fd();
    ssize_t bytes_write = write(fd, send_buf_->c_str(), send_buf_->Size());
    if (bytes_write == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
    return RC_SUCCESS;
}

RC Connection::ReadBlocking() {
    int fd = socket_->fd();
    char buf[1024];
    ssize_t bytes_read = read(fd, buf, sizeof(buf));
    if (bytes_read > 0) {
        recv_buf_->Append(buf, bytes_read);
    } else if (bytes_read == 0) {
        printf("read EOF, blocking client fd:%d disconnected\n", fd);
        state_ = State::Closed;
    } else if (bytes_read == -1) {
        printf("Other error on blocking client fd:%d\n", fd);
        state_ = State::Closed;
    }
    return RC_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////////////////////

RC Connection::ReadNonBlocking() {
    int fd = socket_->fd();

    char buf[1024];
    while (true) {  // because use io nonblocking,reading buf until is finished
        memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            recv_buf_->Append(buf, read_bytes);
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
    return RC_SUCCESS;
}

RC Connection::WriteNonBlocking() {
    int fd = socket_->fd();
    char buf[send_buf_->Size()];
    memcpy(buf, send_buf_->c_str(), send_buf_->Size());

    int data_size = send_buf_->Size();
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
    return RC_SUCCESS;
}

void Connection::set_on_recv(ConnectionMessageCallback const& cb) {
    on_recv_ = std::move(cb);
    ReadCallback bus = std::bind(&Connection::Business, this);
    channel_->set_read_callback(std::move(bus));
}

void Connection::set_send_buf(const char* str) { send_buf_->set_buf(str); }
Buffer* Connection::send_buf() const { return send_buf_.get(); }
Buffer* Connection::recv_buf() const { return recv_buf_.get(); }

