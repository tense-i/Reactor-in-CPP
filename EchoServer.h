#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"
class EchoServer
{
private:
    TcpServer tcpserv_;
    ThreadPool threadpool_;

public:
    EchoServer(const std::string &ip, const uint16_t port, int subThreadNum = 3, int workThreadNum = 5);
    ~EchoServer();
    void Start();
    // 业务函数
    void onMessage(spConnection conn, std ::string &message);
    void newConnectHandler(spConnection clieConnect);
    void closedConnectHandler(spConnection clieConnect);            /*关闭客户端的连接、在Connection中回调它*/
    void errorConnectHandler(spConnection clieConnect);             /*当发生错误时、释放connection*/
    void onMessageHandler(spConnection conn, std::string &message); /* 处理客户端的请求报文、在COnnection类中回调此函数 */
    void sendCompleteHandler(spConnection conn);                    /*数据发送完成后、在connection类中回调此函数*/
    void timeoutHandler(EventLoop *loop);
};