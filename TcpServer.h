#pragma once
#include "Acceptor.h"

#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "ThreadPool.h"
#include "Connection.h"
#include <memory>

#include <map>

class Connection;
class TcpServer
{
private:
    int threadNum_ = 0;
    std::unique_ptr<EventLoop> mainLoop_;              // 主事件2循坏
    std::vector<std::unique_ptr<EventLoop>> subLoops_; // 自事件循环集合
    ThreadPool threadpool_;

    Acceptor acceptor_;                    /*一个TcpServer只有一个acceptor、使用栈内存*/
    std::map<int, spConnection> connects_; /*fd-connection* */

    std::function<void(spConnection)> newConnectCB_;
    std::function<void(spConnection)> closedConnectionCB_;
    std::function<void(spConnection)> errorConnectionCB_;
    std::function<void(spConnection, std::string &massage)> onMessageCB_;
    std::function<void(spConnection)> sendCompleteCB_;
    std::function<void(EventLoop *)> epollTimeoutCB_;

public:
    TcpServer(const std::string &ip, uint16_t port, int threadNum = 3);
    ~TcpServer();
    void start();
    void sendComplete(spConnection conn); /*数据发送完成后、在connection类中回调此函数*/
    void epollTimeout(EventLoop *loop);
    void newConnection(std::unique_ptr<Socket> clieSock);
    void closedConnection(spConnection clieConnect); /*关闭客户端的连接、在Connection中回调它*/
    void errorConnection(spConnection clieConnect);  /*当发生错误时、释放connection*/

    void onMessage(spConnection nconn, std::string &message); /* 处理客户端的请求报文、在COnnection类中回调此函数 */

    void setNewConnectCB(std::function<void(spConnection)> fn);
    void setClosedConnectCB(std::function<void(spConnection)> fn);
    void setErrorConnectCB(std::function<void(spConnection)> fn);
    void setOnMessageCB(std::function<void(spConnection, std::string &message)> fn);
    void setSendCompleteCB(std::function<void(spConnection)> fn);
    void setEpollTimeoutCB(std::function<void(EventLoop *)> fn);
};