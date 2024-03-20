#ifndef _TCPEPOLL_CPP
#define _TCPEPOLL_CPP
#include "inetaddress.h"
#include "Sock.h"
#include "Epoll.h"
#include <vector>

#include <netinet/tcp.h>
#include <sys/epoll.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage::./tcpepoll ip port\n");
    }

    Sock servSocket(createNonblockSock());
    InetAddress servAddr(argv[1], atoi(argv[2]));

    servSocket.setRuseaddr(1);
    servSocket.setReusePort(1);
    servSocket.setKeepAlive(1);
    servSocket.setTCPnodelay(1);

    servSocket.bind(servAddr);
    servSocket.listen();
    Epoll epoll;

    epoll.addFd(servSocket.fd(), EPOLLIN | EPOLLET);
    std::vector<epoll_event> epQue;

    while (true)
    {
        epQue = epoll.loop();

        for (epoll_event &ev : epQue)
        {
            if (ev.events & EPOLLRDHUP) // 对端关闭
            {
                printf("client %d :closed ...\n", ev.data.fd);
                close(ev.data.fd);
            } // 普通读事件或者外带数据
            else if (ev.events & (EPOLLIN | EPOLLHUP))
            {
                // 监听套接字发生读事件--客户端发起请求
                if (ev.data.fd == servSocket.fd())
                {
                    InetAddress clieAddr;

                    // 客户端的clieSock不能再栈上创建、在退栈后会自动退出
                    Sock *clieSock = new Sock(servSocket.accept(clieAddr));

                    printf("new client %s:%d connct...\n", clieAddr.ip(), clieAddr.port());
                    epoll.addFd(clieSock->fd(), EPOLLIN | EPOLLET);
                }
                else // 客户端的fd触发可读事件
                {
                    char buf[1024] = "";
                    ssize_t readn = read(ev.data.fd, buf, sizeof(buf));
                    if (readn > 0)
                    {
                        printf("recv(eventfd=%d)%s\n", ev.data.fd, buf);
                        send(ev.data.fd, buf, strlen(buf), 0);
                    } // 读取数据时被信号中断、继续读取
                    else if (readn == -1 && errno == EINTR)
                    {
                        break;
                    }
                    else if (readn == 0) // 客户端连接断开
                    {
                        printf("client event=%d disconnected \n", ev.data.fd);
                        close(ev.data.fd);
                        break;
                    }
                }
            }
            else if (ev.events & EPOLLOUT)
            {
                ; // redo
            }
            else
            {
                printf("client envent=%d error\n", ev.data.fd);
                close(ev.data.fd);
            }
        }
    }
}

#endif