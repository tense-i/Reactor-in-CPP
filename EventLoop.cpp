#include "EventLoop.h"
#include <sys/syscall.h>

EventLoop::EventLoop() : ep_(new Epoll)
{
}

EventLoop::~EventLoop()
{
    // delete ep_;
}

Epoll *
EventLoop::ep()
{
    return ep_.get();
}

void EventLoop::removeChannel(Channel *ch)
{
    if (ch->inepoll()) // 在树上----MODIFY
    {
        printf("remeve channles\n");
        if ((epoll_ctl(ep_->epfd(), EPOLL_CTL_DEL, ch->fd(), 0)) == -1)
        {
            printf("%s:%s:%d epoll_wait_del failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
}

/**
 * 运行事件循环、并处理已发生事件
 */
void EventLoop::run()
{
    printf("eventloop::run() in thread%lu\n", syscall(SYS_gettid));
    std::vector<Channel *> Channels;

    while (true)
    {
        Channels = ep_->loop(-1);
        if (Channels.size() == 0)
        {
            epollTimeoutCallBack_(this);
        }
        else
        {
            for (auto &ch : Channels)
            {
                ch->eventHandler();
            }
        }
    }
}

void EventLoop::setEpollTimeoutCallBack(std::function<void(EventLoop *)> fn)
{
    epollTimeoutCallBack_ = fn;
}

void EventLoop::upDateChannel(Channel *ch)
{
    epoll_event ev;
    ev.data.ptr = ch;         // 为树节点指定一个绑定的channel
    ev.events = ch->events(); // 更新事件

    if (ch->inepoll()) // 在树上----MODIFY
    {
        if ((epoll_ctl(ep_->epfd(), EPOLL_CTL_MOD, ch->fd(), &ev)) == -1)
        {
            printf("%s:%s:%d epoll_wait failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
    else
    {
        if ((epoll_ctl(ep_->epfd(), EPOLL_CTL_ADD, ch->fd(), &ev)) == -1)
        {
            printf("%s:%s:%d epoll_wait failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
        ch->setInepoll(); // 设置上树状态
    }
}
