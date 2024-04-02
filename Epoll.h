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
    int epfd_ = -1;
    static const int MaxEvents = 1024; // 最大监听事件数
    epoll_event events_[MaxEvents];

public:
    Epoll();
    ~Epoll();

    int epfd();

    /* void addFd(int fd, uint32_t events); */

    /**
     * @brief 运行epoll_waite事件循环、已发生的事件用vector返回
     */
    std::vector<Channel *> loop(int timeout = -1);
};
