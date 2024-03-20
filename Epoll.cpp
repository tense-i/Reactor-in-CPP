#include "Epoll.h"

/* class Epoll
{
private:
    int epfd = -1;
    static const int MaxEvents = 11000;
    epoll_event events[MaxEvents];

public:
    Epoll();
    ~Epoll();
    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> loop(int timeout = -1);
}; */

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
std::vector<epoll_event> Epoll::loop(int timeout = -1)
{
    std::vector<epoll_event> evsQue;
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
        return evsQue;
    }
    for (int i = 0; i < nready; i++)
    {
        evsQue.push_back(evs[i]);
    }
    return evsQue;
}