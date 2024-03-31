#pragma once
#include "Acceptor.h"

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "ThreadPool.h"

#include <map>

class Connection;
class TcpServer
{
private:
    EventLoop *mainLoop_;               // 主事件2循坏
    std::vector<EventLoop *> subLoops_; // 自事件循环集合
    ThreadPool *threadpool_;
    int threadNum_ = 0;
    Acceptor *acceptor_;                   /*一个TcpServer只有一个acceptor*/
    std::map<int, Connection *> connects_; /*fd-connection* */

    std::function<void(Connection *)> newConnectCB_;
    std::function<void(Connection *)> closedConnectionCB_;
    std::function<void(Connection *)> errorConnectionCB_;
    std::function<void(Connection *, std::string &massage)> onMessageCB_;
    std::function<void(Connection *)> sendCompleteCB_;
    std::function<void(EventLoop *)> epollTimeoutCB_;

public:
    TcpServer(const std::string &ip, uint16_t port, int threadNum = 3);
    ~TcpServer();
    void start();
    void sendComplete(Connection *conn); /*数据发送完成后、在connection类中回调此函数*/
    void epollTimeout(EventLoop *loop);
    void newConnection(Socket *clieSock);
    void closedConnection(Connection *clieConnect); /*关闭客户端的连接、在Connection中回调它*/
    void errorConnection(Connection *clieConnect);  /*当发生错误时、释放connection*/

    void onMessage(Connection *conn, std::string &message); /* 处理客户端的请求报文、在COnnection类中回调此函数 */

    void setNewConnectCB(std::function<void(Connection *)> fn);
    void setClosedConnectCB(std::function<void(Connection *)> fn);
    void setErrorConnectCB(std::function<void(Connection *)> fn);
    void setOnMessageCB(std::function<void(Connection *, std::string &message)> fn);
    void setSendCompleteCB(std::function<void(Connection *)> fn);
    void setEpollTimeoutCB(std::function<void(EventLoop *)> fn);
};