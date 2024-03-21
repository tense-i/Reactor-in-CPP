#pragma once
#include "Sock.h"
#include "inetaddress.h"
#include "Channel.h"
#include "EventLoop.h"

class Connection
{
private:
    EventLoop *loop_; // Acceptor对应的事件循环、构造函数中传入
    Sock *clieSocket_;
    Channel *clienChannel_;

public:
    Connection(EventLoop *loop, Sock *clienSock);
    ~Connection();
};
