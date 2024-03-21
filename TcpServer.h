#pragma once
#include "EventLoop.h"
#include "Sock.h"
#include "Channel.h"
#include "Acceptor.h"

class TcpServer
{
private:
    EventLoop loop_;     /*一个TCPserver可以有多个事件循环、现在时单线程，暂时只用一个 事件循环*/
    Acceptor *acceptor_; /*一个TcpServer只有一个acceptor*/

public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start();
    void newConnect(Sock *clieSock);
};