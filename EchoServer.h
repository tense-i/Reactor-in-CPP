#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"

class EchoServer
{
private:
    TcpServer tcpserv_;

public:
    EchoServer(const std::string &ip, const uint16_t port);
    ~EchoServer();
    void Start();
    void newConnectHandler(Connection *clieConnect);
    void closedConnectHandler(Connection *clieConnect);            /*关闭客户端的连接、在Connection中回调它*/
    void errorConnectHandler(Connection *clieConnect);             /*当发生错误时、释放connection*/
    void onMessageHandler(Connection *conn, std::string &message); /* 处理客户端的请求报文、在COnnection类中回调此函数 */
    void sendCompleteHandler(Connection *conn);                    /*数据发送完成后、在connection类中回调此函数*/
    void timeoutHandler(EventLoop *loop);
};