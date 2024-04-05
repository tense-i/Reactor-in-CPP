#include "EventLoop.h"
#include <sys/syscall.h>

EventLoop::EventLoop() : ep_(new Epoll), wakeupfd_(eventfd(0, EFD_NONBLOCK)), wakeupChannel_(new Channel(this, wakeupfd_))
{
    wakeupChannel_->setReadCallBack(std::bind(&EventLoop::wakeupHeadler, this));
    wakeupChannel_->enableReading();
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

    tid_ = syscall(SYS_gettid); // 获取事件循环所在线程的tid
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

bool EventLoop::isInLoopthread()
{
    return tid_ == syscall(SYS_gettid);
}

void EventLoop::pushInloop(std::function<void()> fn)
{
    // 加入任务给IO线程工作
    {
        std::lock_guard<std::mutex> gd(mutex_);
        taskQueue_.push(fn);
    }

    // 唤醒阻塞的IO线程工作
    wakeup();
}

void EventLoop::wakeup()
{
    uint64_t val = 1;
    write(wakeupfd_, &val, sizeof(val));
}

void EventLoop::wakeupHeadler()
{

    {
        std::function<void()> fn;
        std::lock_guard<std::mutex> gd(mutex_);
        uint64_t val;
        read(wakeupfd_, &val, sizeof(val)); // 从eventfd中读取出数据、如果不读取、eventfd会一直被触发
        printf("wakeup thead id is %lu val=%ld\n", syscall(SYS_gettid), val);

        while (taskQueue_.size() > 0)
        {
            fn = std::move(taskQueue_.front());
            taskQueue_.pop();
            fn(); // 当前IO线程执行任务
        }
    }
}
