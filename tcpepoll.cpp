#ifndef _TCPEPOLL_CPP
#define _TCPEPOLL_CPP
#include "inetaddress.h"
#include "Sock.h"
#include "Epoll.h"
#include <vector>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include "EventLoop.h"
#include "TcpServer.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage::./tcpepoll ip port\n");
    }
    TcpServer serv(argv[1], atoi(argv[2]));

    serv.start();

    return 0;
}

#endif