#include "Channel.h"
#include "InetAddress.h"
#include "Connection.h"

Channel::Channel(EventLoop *evloop, int fd) : evloop_(evloop), fd_(fd)
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
    evloop_->ep()->upDateChannel(this);
}

void Channel::disableReading()
{
    events_ &= (~EPOLLIN);
    evloop_->ep()->upDateChannel(this);
}

void Channel::enableWrite()
{
    events_ = (events_ | EPOLLOUT);
    evloop_->ep()->upDateChannel(this);
}

void Channel::disableWrite()
{
    events_ &= (~EPOLLOUT);
    evloop_->ep()->upDateChannel(this);
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
    }
}

/* void Channel::onMessage()
{
    char buf[1024];
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        int nread = read(fd_, buf, sizeof(buf));

        if (nread > 0)
        {
            printf("server(event %d): %s\n", fd_, buf);
            write(fd_, buf, nread);
        } // 信号中断
        else if (nread == -1 && (errno == EINTR))
        {
            continue;
        } // 数据读取完毕
        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            break;
        }
        else if (nread == 0) // 客户端半关闭
        {
            closedCallBack_();
        }
    }
} */

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
