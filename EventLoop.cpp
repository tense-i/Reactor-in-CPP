#include "EventLoop.h"
#include <sys/syscall.h>

EventLoop::EventLoop() : ep_(new Epoll)
{
}

EventLoop::~EventLoop()
{
    // delete ep_;
}

/* Epoll *
EventLoop::ep()
{
    return ep_.get();
} */

void EventLoop::removeChannel(Channel *ch)
{
    ep_->removeChannel(ch);
}

/**
 * 运行事件循环、并处理已发生事件
 */
void EventLoop::run()
{
    // printf("eventloop::run() in thread%lu\n", syscall(SYS_gettid));
    ;

    while (true)
    {
        std::vector<Channel *> Channels = ep_->loop(-1);
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
    ep_->upDateChannel(ch);
}
