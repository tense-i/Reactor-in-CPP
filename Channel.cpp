#include "Channel.h"
#include "inetaddress.h"
#include "Sock.h"
#include <sys/epoll.h>
#include "EventLoop.h"
#include "Epoll.h"
#include "Connection.h"
Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd)
{
}

Channel::~Channel()
{
    // 不要销毁loop_,也不能关闭fd_因为这两个东西不属于Channel类、channel只管辖它们o
}

int Channel::fd()
{
    return fd_;
}

/**
 * @brief 设为边缘触发（ET)*/
void Channel::useET()
{
    events_ = events_ | EPOLLET;
}

/*监视读事件*/
void Channel::enableReading()
{
    events_ = events_ | EPOLLIN;
    loop_->ep()->updateChannel(this);
}

void Channel::disableReading()
{
    events_ &= (~EPOLLIN);
    loop_->ep()->updateChannel(this);
}

void Channel::enableWrite()
{
    events_ |= EPOLLOUT;
    loop_->ep()->updateChannel(this);
}

void Channel::disableWrite()
{
    events_ &= (~EPOLLOUT);
    loop_->ep()->updateChannel(this);
}

/*设置inepoll的值*/
void Channel::setInepoll()
{
    inEpoll_ = true;
}

/*设置发生的事件*/
void Channel::setRevents(uint32_t event)
{
    revents_ = revents_ | event;
}

/*返回inepoll的值*/
bool Channel::inepoll()
{
    return inEpoll_;
}
/*返回监听事件类型*/
uint32_t Channel::events()
{
    return events_;
}

/*返回已发生事件类型*/
uint32_t Channel::revents()
{
    return revents_;
}

int Channel::handlerEvent()
{
    if (this->events_ & EPOLLRDHUP) // 对端关闭
    {
        closedCallBack_();
    } // 普通读事件或者外带数据
    else if (events_ & (EPOLLIN | EPOLLHUP))
    {
        readCallBack_();
    }
    else if (fd_ & EPOLLOUT) // 普通写事件
    {
        writeCallBack_();
    }
    else
    {
        errorCallBack_();
    }
    return 0;
}

void Channel::setReadCallBack(std::function<void()> fn)
{
    readCallBack_ = fn;
}

void Channel::setClosedCallBack(std::function<void()> fn)
{
    closedCallBack_ = fn;
}

void Channel::setErrorCallBack(std::function<void()> fn)
{
    errorCallBack_ = fn;
}

void Channel::setWriteCallBack(std::function<void()> fn)
{
    writeCallBack_ = fn;
}
