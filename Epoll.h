#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include "Channel.h"
class Channel;
class Epoll
{
private:
    int epfd = -1;
    static const int MaxEvents = 11000;
    epoll_event evs[MaxEvents];

public:
    Epoll();
    ~Epoll();
    void updateChannel(Channel *ch); /*添加、或者更新channel到红黑树*/
    void addFd(int fd, uint32_t events);
    std::vector<Channel *> loop(int timeout = -1);
};