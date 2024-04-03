#include "Channel.h"
#include "InetAddress.h"
#include "Connection.h"

Channel::Channel(std::unique_ptr<EventLoop> &evloop, int fd) : evloop_(evloop), fd_(fd)
{
}

Channel::~Channel()
{
    // 不能释放channel类的fd与ev、channel只是使用它们、而不属于channel
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
    evloop_->upDateChannel(this);
}

void Channel::disableReading()
{
    events_ &= (~EPOLLIN);
    evloop_->upDateChannel(this);
}

void Channel::enableWrite()
{
    events_ = (events_ | EPOLLOUT);
    evloop_->upDateChannel(this);
}

void Channel::disableWrite()
{
    events_ &= (~EPOLLOUT);
    evloop_->upDateChannel(this);
}

/*设置inepoll的值*/
void Channel::setInepoll()
{
    inEpoll_ = true;
}

/*设置发生的事件*/
void Channel::setRevents(uint32_t event)
{
    revents_ = event;
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

/*事件处理函数*/
void Channel::eventHandler()
{

    if (revents_ & EPOLLRDHUP) // 客户端关闭
    {
        printf("EPOLLRDHUP\n");
        // 客户端关闭、注销管理器channel
        // remove();
        closedCallBack_();
    }
    else if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        printf("EPOLLIN\n");
        readCallBack_();
    }
    else if (revents_ & EPOLLOUT)
    {
        printf("EPOLLOUT\n");
        writeCallBack_();
    }
    else
    {
        printf("ERROR\n");
        errorCallBack_();
        // 客户端error、注销管理器channel
        // remove();
    }
}

void Channel::disableAllEvent()
{
    events_ = 0;
    evloop_->upDateChannel(this);
}

void Channel::remove()
{
    disableAllEvent();
    evloop_->removeChannel(this); // 从epoll的红黑树上删除该channel管理的fd
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
