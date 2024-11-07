#include "Connection.h"

#include <unistd.h>

#include <cstring>
#include <iostream>

#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop* loop, Socket* s) : loop_{loop}, s_{s} {
  ch_ = new Channel(loop_, s->getFd());
  ch_->useET();
  ch_->enableReading();

  std::function<void()> cb = std::bind(&Connection::echo, this, s_->getFd());
  ch_->setReadCallback(cb);
  // ch_->setUseThreadPool( true );

  readBuffer_ = new Buffer();
  writeBuffer_ = new Buffer();
}

Connection::~Connection() {
  delete ch_;
  delete s_;

  delete readBuffer_;
  delete writeBuffer_;
}

void Connection::echo(int fd) {
  constexpr auto READ_BUFFER = 1024;
  char buf[READ_BUFFER];
  while (true) {  // because use io nonblocking,
    ssize_t read_bytes = read(fd, buf, sizeof(buf));
    if (read_bytes > 0) {
      readBuffer_->append(buf, read_bytes);
      // std::cout << "message from client fd:" << fd << "[" << buf <<
      // "]\n";
    } else if (read_bytes == -1 && errno == EINTR) {
      std::cout << "continue reading\n";
      continue;
    } else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      std::cout << "message from client fd:" << fd << " ["
                << readBuffer_->c_str() << "]\n";
      send(fd);
      readBuffer_->clear();
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

void Connection::send(int fd) {
  char buf[readBuffer_->size()];
  strcpy(buf, readBuffer_->c_str());
  int data_size = readBuffer_->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(fd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    data_left -= bytes_write;
  }
}
