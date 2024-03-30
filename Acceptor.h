#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"

/**
 * @brief 提取器、为监听socket服务
 */
class Acceptor
{
private:
    EventLoop *evloop_; // Acceptor对应的事件循环、构造函数中传入
    Socket *servSocket_;
    Channel *acceptChannel_;
    std::function<void(Socket *)> newConnectCallBack_;

public:
    Acceptor(EventLoop *evloop, const std::string &ip, const uint16_t port);
    ~Acceptor();
    void newConnection();
    void setNewConnectCallBack(std::function<void(Socket *)> fn);
};
