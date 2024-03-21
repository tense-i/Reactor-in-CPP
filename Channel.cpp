#include "Channel.h"
#include "inetaddress.h"
#include "Sock.h"
#include <sys/epoll.h>

Channel::Channel(Epoll *ep, int fd) : ep_(ep), fd_(fd)
{
}

Channel::~Channel()
{
    // 不能释放channel类的fd与ev、channel只是使用它们
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
    ep_->updateChannel(this);
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
        printf("client %d :closed ...\n", fd_);
        close(fd_);
    } // 普通读事件或者外带数据
    else if (events_ & (EPOLLIN | EPOLLHUP))
    {
        readCallBack_();
    }
    else if (fd_ & EPOLLOUT)
    {
        ; // redo
    }
    else
    {
        printf("client envent=%d error\n", fd_);
        close(fd_);
    }
    return 0;
}

void Channel::newConnect(Sock *servSock)
{
    InetAddress clieAddr;

    // 客户端的clieSock不能再栈上创建、在退栈后会自动退出、调用析构、释放fd
    Sock *clieSock = new Sock(servSock->accept(clieAddr));

    printf("new client %s:%d connct...\n", clieAddr.ip(), clieAddr.port());

    Channel *clieChannel = new Channel(ep_, clieSock->fd());
    clieChannel->useET();
    clieChannel->enableReading();
    clieChannel->setReadCallBack(std::bind(&Channel::onMessageArvc, clieChannel));
}

void Channel::onMessageArvc()
{
    char buf[1024];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t nread = read(fd_, buf, sizeof(buf));
        if (nread > 0)
        {
            printf("recv(event=%d):%s\n", fd_, buf);
            send(fd_, buf, strlen(buf), 0);
        } // 读取数据时被中断、继续读取
        else if (nread == -1 && errno == EINTR)
        {
            continue;
        } // 全部数据已经读取完毕
        else if (nread == -1 && ((errno == EAGAIN)) || (errno == EWOULDBLOCK))
        {
            break;
        }
        else if (nread == 0)
        {
            printf("client(eventfddd =%d ) disconnected\n", fd_);
            close(fd_);
            break;
        }
    }
}

void Channel::setReadCallBack(std::function<void()> fn)
{
    readCallBack_ = fn;
}

void Channel::readCallBack()
{
}
