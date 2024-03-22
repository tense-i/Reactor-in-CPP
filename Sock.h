#ifndef SOCK_H
#define SOCK_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "inetaddress.h"

int createNonblockSock();

class Sock
{
private:
    const int fd_;
    std::string ip_;
    uint16_t port_;

public:
    Sock();
    Sock(int fd);
    ~Sock();
    int fd() const;
    void setRuseaddr(bool on);
    void setTCPnodelay(bool on = 0);
    void setKeepAlive(bool on = 0);
    void setReusePort(bool on = 0);
    std::string ip();
    uint16_t port();

    void bind(const InetAddress &addr);
    void listen(int queSize = 128);
    int accept(InetAddress &addr);
};

#endif