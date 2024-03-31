#include "EventLoop.h"
#include <sys/syscall.h>

EventLoop::EventLoop() : ep_(new Epoll)
{
}

EventLoop::~EventLoop()
{
    delete ep_;
}

Epoll *
EventLoop::ep()
{
    return ep_;
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
