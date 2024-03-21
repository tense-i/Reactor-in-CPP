#include "EventLoop.h"

EventLoop::EventLoop() : ep_(new Epoll)
{
}

EventLoop::~EventLoop()
{
    delete ep_;
}

Epoll *EventLoop::ep()
{
    return ep_;
}

void EventLoop::run()
{
    std::vector<Channel *> Channels;

    while (true)
    {
        Channels = ep_->loop();

        for (auto &ch : Channels)
        {
            ch->handlerEvent();
        }
    }
}
