#include "Epoll.h"

#include <unistd.h>
#include <cstring>
#include "Channel.h"
#include "util.h"

constexpr auto MAX_EVENTS = 1000;

Epoll::Epoll() : epfd{-1}, events{nullptr} {
  epfd = epoll_create1(0);
  ErrIf(epfd == -1, "epoll create error");
  events = new epoll_event[MAX_EVENTS];
  bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
  if (epfd != -1) {
    close(epfd);
  }
  delete[] events;
}

std::vector<Channel *> Epoll::poll(int timeout) {
  int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
  ErrIf(nfds == -1, "epoll wait error");
  std::vector<Channel *> activeChannels;
  for (int i = 0; i != nfds; ++i) {
    Channel *ch = (Channel *)events[i].data.ptr;
    ch->setReady(events[i].events);
    activeChannels.emplace_back(ch);
  }
  return activeChannels;
}

void Epoll::updateChannel(Channel *ch) {
  int fd = ch->getFd();
  struct epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.ptr = ch;
  ev.events = ch->getEvents();
  if (!ch->getInEpoll()) {
    ErrIf(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    ch->setInEpoll();
  } else {
    ErrIf(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

void Epoll::deleteChannel(Channel *ch) {
  int fd = ch->getFd();
  ErrIf(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
  ch->setInEpoll(false);
}
