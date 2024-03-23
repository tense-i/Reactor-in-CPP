#include "EventLoop.h"

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

void EventLoop::setEpollTimeoutCallBack(std::function<void(EventLoop *)> fn)
{
    epollTimeoutCallBack_ = fn;
}

void EventLoop::run()
{
    std::vector<Channel *> Channels;

    while (true)
    {
        Channels = ep_->loop();
        // 如果channels为空、则表示超时、回调TCPServer::epolltimeout();
        if (Channels.size() == 0)
        {
            epollTimeoutCallBack_(this);
        }
        else
        {
            for (auto &ch : Channels)
            {
                ch->handlerEvent();
            }
        }
    }
}
