#pragma once
#include "Epoll.h"
#include <functional>
#include "Channel.h"
#include <memory>
#include "ThreadPool.h"
#include <mutex>
#include <sys/eventfd.h>

class Epoll;
class Channel;
class EventLoop
{
private:
    std::unique_ptr<Epoll> ep_;
    std::function<void(EventLoop *)> epollTimeoutCallBack_;
    pid_t tid_;
    std::queue<std::function<void()>> taskQueue_;
    std::mutex mutex_;
    int wakeupfd_;
    std::unique_ptr<Channel> wakeupChannel_;

public:
    EventLoop();
    ~EventLoop();
    void removeChannel(Channel *ch); /*从epoll的红黑树上删除该channel管理的fd*/
    void run();
    void setEpollTimeoutCallBack(std::function<void(EventLoop *)> fn);
    /**
     * @brief 将channel添加、更新到epoll树上，channel用户管理树节点、事件触发.做了原本addFd函数的事情
     */
    void upDateChannel(Channel *ch);
    bool isInLoopthread(); // 判断当前线程是否为事件循环线程
    void pushInloop(std::function<void()> fn);
    void wakeup();        // 用eventfd唤醒事件循环线程
    void wakeupHeadler(); // 事件循环线程被eventfd唤醒后执行的函数
};
