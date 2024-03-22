#pragma once
#include <sys/epoll.h>
#include <functional>
#include "EventLoop.h"

class EventLoop;
class Sock;

class Channel // channel时accept与connectoin的下层类
{
private:
    int fd_ = -1;               // Channel与fd是一对一的关系
    EventLoop *loop_ = nullptr; // Channel对应 红黑树、与EPoll是多对一的关系、一个Channel只对应一个Epoll
    bool inEpoll_ = false;      // channnel是否已经添加到红黑树上，用于调整epoll_ctL的opt
    uint32_t events_ = 0;       // 监视的事件
    uint32_t revents_ = 0;      // 已经触发的事件

    std::function<void()> readCallBack_;
    std::function<void()> closedCallBack_;
    std::function<void()> errorCallBack_;

public:
    Channel(EventLoop *loop, int fd);
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
    void onMessageArvc();
    void setReadCallBack(std::function<void()> fn);
    void setClosedCallBack(std::function<void()> fn);
    void setErrorCallBack(std::function<void()> fn);
};
