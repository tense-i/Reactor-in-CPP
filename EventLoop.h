#pragma once

#include "Epoll.h"

class EventLoop
{
private:
    Epoll *ep_;

public:
    EventLoop();
    ~EventLoop();
    Epoll *ep();

    void run();
};