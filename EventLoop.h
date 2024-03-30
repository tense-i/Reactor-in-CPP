#pragma once
#include "Epoll.h"
#include <functional>

class Epoll;

class EventLoop
{
private:
    Epoll *ep_;
    std::function<void(EventLoop *)> epollTimeoutCallBack_;

public:
    EventLoop();
    ~EventLoop();
    Epoll *ep();

    void run();
    void setEpollTimeoutCallBack(std::function<void(EventLoop *)> fn);
};