#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include <sys/epoll.h>
#include <functional>
#include <memory>

class Epoll;
class EventLoop;

/**
 * @brief Channel相当于xx_event结构体、用来管理事件的
 */
class Channel
{
private:
    int fd_ = -1;          // Channel与fd是一对一的关系
    EventLoop *evloop_;    // Channel对应 红黑树、与EPoll是多对一的关系、一个Channel只对应一个Epoll
    bool inEpoll_ = false; // channnel是否已经添加到红黑树上，用于调整epoll_ctL的opt
    uint32_t events_ = 0;  // 监视的事件
    uint32_t revents_ = 0; // 已经触发的事件

    std::function<void()> readCallBack_;
    std::function<void()> closedCallBack_;
    std::function<void()> errorCallBack_;
    std::function<void()> writeCallBack_;

public:
    Channel(EventLoop *evloop, int fd);
    ~Channel();
    int fd();
    void useET();                    /*设为边缘触发*/
    void enableReading();            /*监视读事件*/
    void disableReading();           /*取消读事件*/
    void enableWrite();              /*监视写事件*/
    void disableWrite();             /*取消写事件*/
    void setInepoll();               /*设置inepoll的值*/
    void setRevents(uint32_t event); /*设置发生的事件*/
    bool inepoll();                  /*返回inepoll的值*/
    uint32_t events();               /*返回监听事件类型*/
    uint32_t revents();              /*返回已发生事件类型*/
    void eventHandler();             /*事件处理函数*/
    void disableAllEvent();          /*取消监听所有事件*/
    void remove();                   /*当连接断开时、channel已经没有存在的必要*/
    void setReadCallBack(std::function<void()> fn);
    void setClosedCallBack(std::function<void()> fn);
    void setErrorCallBack(std::function<void()> fn);
    void setWriteCallBack(std::function<void()> fn);
    // void onMessage(); /*接受报文*/
};
