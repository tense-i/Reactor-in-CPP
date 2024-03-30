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

    /* void addFd(int fd, uint32_t events); */

    /**
     * @brief 将channel添加、更新到epoll树上，channel用户管理树节点、事件触发.做了原本addFd函数的事情
     */
    void upDateChannel(Channel *ch);
    /**
     * @brief 运行epoll_waite事件循环、已发生的事件用vector返回
     */
    std::vector<Channel *> loop(int timeout = -1);
};
