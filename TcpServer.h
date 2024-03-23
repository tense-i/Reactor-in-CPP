#pragma once
#include "EventLoop.h"
#include "Sock.h"
#include "Channel.h"
#include "Acceptor.h"
#include <map>
#include "Connection.h"

class TcpServer
{
private:
    EventLoop loop_;     /*一个TCPserver可以有多个事件循环、现在时单线程，暂时只用一个 事件循环*/
    Acceptor *acceptor_; /*一个TcpServer只有一个acceptor*/
    std::map<int, Connection *> connects_;

public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start();
    void newConnect(Sock *clieSock);
    void closeClientConnect(Connection *clieConnect);          /*关闭客户端的连接、在Connection中回调它*/
    void onError_closedClientConnect(Connection *clieConnect); /*当发生错误时、释放connection*/
    void onMessage(Connection *conn, std::string message);     /* 处理客户端的请求报文、在COnnection类中回调此函数 */
    void sendComplete(Connection *conn);                       /*数据发送完成后、在connection类中回调此函数*/
    void epollTimeout(EventLoop *loop);
};