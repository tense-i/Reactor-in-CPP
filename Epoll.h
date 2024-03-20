#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

class Epoll
{
private:
    int epfd = -1;
    static const int MaxEvents = 11000;
    epoll_event evs[MaxEvents];

public:
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t events);
    std::vector<epoll_event> loop(int timeout = -1);
};