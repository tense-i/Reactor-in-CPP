#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <memory>
/**
 * @brief 提取器、为监听socket服务
 */
class Acceptor
{
private:
    // 传递智能指针引用、accepor对evloop_没有所属权、它属于Tcpserver
    std::unique_ptr<EventLoop> &evloop_; // Acceptor对应的事件循环、构造函数中传入
    Socket servSocket_;
    Channel acceptChannel_; // 一个服务器只有一个acceptChannel_适合放在栈区
    // 占用内存少、而且指针不好管理、用栈内存更好！！
    // Socket *servSocket_;
    // Channel *acceptChannel_;
    std::function<void(std::unique_ptr<Socket>)> newConnectCallBack_;

public:
    Acceptor(std::unique_ptr<EventLoop> &evloop, const std::string &ip, const uint16_t port);
    ~Acceptor();
    void newConnection();
    void setNewConnectCallBack(std::function<void(std::unique_ptr<Socket>)> fn);
};
