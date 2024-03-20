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

std::vector<Channel *> Epoll::loop(int timeout)
{
    std::vector<Channel *> Channels;
    bzero(evs, sizeof(evs));

    int nready = epoll_wait(epfd, evs, MaxEvents, -1);
    if (nready < 0)
    {
        printf("%s:%s:%d epoll_waite\n", __FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }

    if (nready == 0)
    {
        printf("%s:%s:%d epoll_waite\n", __FILE__, __FUNCTION__, __LINE__);
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