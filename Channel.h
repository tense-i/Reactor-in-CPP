#pragma once
#include <sys/epoll.h>
#include "Epoll.h"
#include <functional>

class Epoll;
class Sock;

class Channel
{
private:
    int fd_ = -1;          // Channel与fd是一对一的关系
    Epoll *ep_ = nullptr;  // Channel对应 红黑树、与EPoll是多对一的关系、一个Channel只对应一个Epoll
    bool inEpoll_ = false; // channnel是否已经添加到红黑树上，用于调整epoll_ctL的opt
    uint32_t events_ = 0;  // 监视的事件
    uint32_t revents_ = 0; // 已经触发的事件

    std::function<void()> readCallBack_;

public:
    Channel(Epoll *ep, int fd);
    ~Channel();
    int fd();
    void useET();                    /*设为边缘触发*/
    void enableReading();            /*监视读事件*/
    void setInepoll();               /*设置inepoll的值*/
    void setRevents(uint32_t event); /*设置发生的事件*/
    bool inepoll();                  /*返回inepoll的值*/
    uint32_t events();               /*返回监听事件类型*/
    uint32_t revents();              /*返回已发生事件类型*/
    int handlerEvent();              /*事件处理函数*/
    void newConnect(Sock *servSock); /*处理新客户端连接*/
    void onMessageArvc();
    void setReadCallBack(std::function<void()> fn);
    void readCallBack();
};
