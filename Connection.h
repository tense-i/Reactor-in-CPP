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
    std::function<void(Connection *)> closeCallBack_; /*关闭fd_的回调函数、将回调Tcpserver::closedconnecion*/
    std::function<void(Connection *)> errorCallBack_;

public:
    Connection(EventLoop *loop, Sock *clienSock);
    ~Connection();
    int fd() const;
    std::string ip() const;
    uint16_t port() const;
    void errorCallBack();
    void closedCallBack();
    void setClosedCallBack(std::function<void(Connection *)> fn);
    void setErrorCallBack(std::function<void(Connection *)> fn);
};
