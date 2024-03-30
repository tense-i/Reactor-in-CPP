#include "Socket.h"
#include "InetAddress.h"
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

Socket::Socket(int fd) : fd_(fd) {}

Socket::Socket() : fd_(0) {}

Socket::~Socket() { close(fd_); }

int Socket::fd() const
{
    return fd_; // fd_是const的常函数是可以访问的
}

uint16_t Socket::port() const
{
    return port_;
}

std::string Socket::ip() const
{
    return ip_;
}

void Socket::setRuseaddr(bool on)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Socket::setTCPnodelay(bool on)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Socket::setKeepAlive(bool on)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));
}
void Socket::setReusePort(bool on)
{
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
}

void Socket::bind(const InetAddress &addr)
{
    if (::bind(fd_, addr.addr(), sizeof(sockaddr)) < 0)
    {
        printf("%s:%s:%d bind socket  error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    ip_ = addr.ip();
    port_ = addr.port();
}

void Socket::listen(int queSize)
{
    if (::listen(fd_, queSize) < 0)
    {
        printf("%s:%s:%d listen socket error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
}

/**
 * @param clieAddr 传入型参数-为客户端的地址结构体赋值
 */
int Socket::accept(InetAddress &clieAddr)
{
    sockaddr_in peerAddr;
    socklen_t len = sizeof(peerAddr);
    int cliefd = accept4(fd_, (sockaddr *)&peerAddr, &len, SOCK_NONBLOCK);
    clieAddr.setAddr(peerAddr);
    return cliefd;
}

/**
 * @brief 创建非阻塞的监听套接字、不是Soket类的成员函数、是全局函数
 */
int createNonblockSocket()
{
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        printf("%s:%s:%d listen socket create  error:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    return listenfd;
}
