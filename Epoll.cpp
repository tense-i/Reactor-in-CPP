#include "Epoll.h"

void Epoll::updateChannel(Channel *ch) /*添加、或者更新channel到红黑树*/
{
    epoll_event ev;
    ev.events = ch->events();
    ev.data.ptr = ch;
    // 判断该节点是否已经在epoll树上
    if (ch->inepoll())
    {
        if (epoll_ctl(epfd, (EPOLL_CTL_MOD), ch->fd(), &ev) == -1)
        {
            printf("%s:%s:%d epoll ctl modify erro \n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
    }
    else
    {
        if (epoll_ctl(epfd, (EPOLL_CTL_ADD), ch->fd(), &ev) == -1)
        {
            printf("%s:%s:%d epoll ctl add erro\n", __FILE__, __FUNCTION__, __LINE__);
            exit(-1);
        }
        else
        {
            ch->setInepoll();
        }
    }
}

Epoll::Epoll()
{
    epfd = epoll_create1(0);
    if (epfd == -1)
    {
        printf("%s:%s:%d epoll create filed\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    close(epfd);
}

void Epoll::addFd(int fd, uint32_t events)
{
    epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    if ((epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)) == -1)
    {
        printf("%s:%s:%d epoll_ctl filedd\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
}

std::vector<Channel *> Epoll::loop(int timeout = -1)
{
    std::vector<Channel *> Channels;
    bzero(evs, sizeof(evs));

    int nready = epoll_wait(epfd, evs, MaxEvents, timeout);
    if (nready < 0)
    {
        // EBADF: epfd不是一个有效描述符
        // EFAULT:参数events指向的内存区、不可写
        // EINVAL:epfd不是一个epoll文件描述符、或者参数maxevents小于等于0
        // EINTR: 阻塞过程中被信号中断、epoll_wait可以避免、解析error后重新调用epool_wait
        // 在reactor模型中、不建议使用信号、因为信号的处理很麻烦、没有必要--陈硕
        printf("%s:%s:%d epoll_waite\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }

    if (nready == 0) // 超时
    {
        // printf("%s:%s:%d epoll_waite\n", __FILE__, __FUNCTION__, __LINE__);
        return Channels;
    }
    for (int i = 0; i < nready; i++)
    {
        Channel *ch = (Channel *)evs[i].data.ptr;
        ch->setRevents(evs[i].events);
        Channels.push_back(ch);
    }
    return Channels;
}