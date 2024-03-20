
#include "Sock.h"
#include "inetaddress.h"
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

/* class Sock
{
private:
    const int fd_;

public:
    Sock();
    ~Sock();
    int fd() const;
    void setRuseaddr(bool on);
    void bind(const InetAddress &addr);
    void listen(int queSize = 128);
    void accept(const InetAddress &addr);
};
 */

Sock::Sock(int fd) : fd_(fd) {}

Sock::Sock() : fd_(0) {}

Sock::~Sock() { close(fd_); }

int Sock::fd() const
{
    return fd_; // fd_是const的常函数是可以访问的
}

void Sock::setRuseaddr(bool on)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Sock::setTCPnodelay(bool on = 0)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Sock::setKeepAlive(bool on = 0)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));
}
void Sock::setReusePort(bool on = 0)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Sock::bind(const InetAddress &addr)
{
    if (::bind(fd_, addr.addr(), sizeof(sockaddr)) < 0)
    {
        printf("%s:%s:%d bind socket  error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
}

void Sock::listen(int queSize = 128)
{
    if (::listen(fd_, queSize) < 0)
    {
        printf("%s:%s:%d listen socket error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
}
int Sock::accept(InetAddress &addr)
{
    sockaddr_in peerAddr;
    socklen_t len = sizeof(peerAddr);
    int cliefd = accept4(fd_, (sockaddr *)&peerAddr, &len, SOCK_NONBLOCK);

    addr.setAddr(peerAddr);
    return cliefd;
}

int createNonblockSock()
{
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        printf("%s:%s:%d listen socket create  error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    return listenfd;
}