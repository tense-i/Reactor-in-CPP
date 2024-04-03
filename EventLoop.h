#pragma once
#include "Epoll.h"
#include <functional>
#include "Channel.h"
#include <memory>

class Epoll;
class Channel;
class EventLoop
{
private:
    std::unique_ptr<Epoll> ep_;
    std::function<void(EventLoop *)> epollTimeoutCallBack_;

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
};