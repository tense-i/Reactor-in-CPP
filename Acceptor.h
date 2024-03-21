#pragma once
#include "Sock.h"
#include "inetaddress.h"
#include "Channel.h"
#include "EventLoop.h"

class Acceptor
{
private:
    EventLoop *loop_; // Acceptor对应的事件循环、构造函数中传入
    Sock *servSocket_;
    Channel *acceptChannel_;

public:
    Acceptor(EventLoop *loop, const std::string &ip, const uint16_t port);
    ~Acceptor();
};
