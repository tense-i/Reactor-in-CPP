#include "Epoll.h"
#include "Channel.h"

Epoll::Epoll()
{
    epfd_ = epoll_create(1);
    if (epfd_ == -1)
    {
        printf("%s:%s:%d epoll create failed\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    close(epfd_);
}

int Epoll::epfd()
{
    return epfd_;
}

/**
 * @brief 向epoll红黑树上添加事件
 */
/* void Epoll::addFd(int fd, uint32_t events)
{
    epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    if ((epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev)) == -1)
    {
        printf("%s:%s:%d epoll_ctl filedd\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
}
 */
std::vector<Channel *> Epoll::loop(int timeout)
{
    std::vector<Channel *> channels;
    bzero(events_, sizeof(events_));

    int nReady = epoll_wait(epfd_, events_, MaxEvents, timeout);
    if (nReady < 0)
    {
        // EBAFD epfd不是有效描述符
        // EFAULT events指向的内存区域不可写
        // EINVAL epdf不是一个epoll文件描述符

        printf("%s:%s:%d epoll_wait failed\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }

    // 超时
    if (nReady == 0)
    {
        // printf("epoll_wait timeout\n");
        return channels;
    }

    for (int i = 0; i < nReady; i++)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->setRevents(events_[i].events); // 关键！！！更新发生的事件、在eventhandler要用、否则不触发事件
        channels.push_back(ch);
    }
    return channels;
}

/* void Epoll::upDateChannel(Channel *ch)
{
    epoll_event ev;
    ev.data.ptr = ch;         // 为树节点指定一个绑定的channel
    ev.events = ch->events(); // 更新事件

    if (ch->inepoll()) // 在树上----MODIFY
    {
        if ((epoll_ctl(epfd_, EPOLL_CTL_MOD, ch->fd(), &ev)) == -1)
        {
            printf("%s:%s:%d epoll_wait failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
    else
    {
        if ((epoll_ctl(epfd_, EPOLL_CTL_ADD, ch->fd(), &ev)) == -1)
        {
            printf("%s:%s:%d epoll_wait failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
        ch->setInepoll(); // 设置上树状态
    }
} */

/* void Epoll::removeChannel(Channel *ch)
{

    if (ch->inepoll()) // 在树上----MODIFY
    {
        printf("remeve channles\n");
        if ((epoll_ctl(epfd_, EPOLL_CTL_DEL, ch->fd(), 0)) == -1)
        {
            printf("%s:%s:%d epoll_wait_del failed\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
}
 */