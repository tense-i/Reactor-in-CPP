#include "EchoServer.h"
#include <sys/syscall.h>

EchoServer::EchoServer(const std::string &ip, const uint16_t port, int subThreadNum, int workThreadNum) : tcpserv_(ip, port, subThreadNum), threadpool_(workThreadNum, "WORK")
{
    // 一下代码非必要、看业务需求
    tcpserv_.setClosedConnectCB(std::bind(&EchoServer::closedConnectHandler, this, std::placeholders::_1));
    tcpserv_.setEpollTimeoutCB(std::bind(&EchoServer::timeoutHandler, this, std::placeholders::_1));
    tcpserv_.setNewConnectCB(std::bind(&EchoServer::newConnectHandler, this, std::placeholders::_1));
    tcpserv_.setOnMessageCB(std::bind(&EchoServer::onMessageHandler, this, std::placeholders::_1, std::placeholders::_2));
    tcpserv_.setSendCompleteCB(std::bind(&EchoServer::sendCompleteHandler, this, std::placeholders::_1));
    tcpserv_.setErrorConnectCB(std::bind(&EchoServer::errorConnectHandler, this, std::placeholders::_1));
}

EchoServer::~EchoServer()
{
}

void EchoServer::Start()
{
    tcpserv_.start();
}

void EchoServer::onMessage(Connection *conn, std::string &message)
{
    message = "reply:" + message;
    int len = message.size();
    conn->send(message.data(), message.size());
}

//
void EchoServer::newConnectHandler(Connection *clieConnect)
{
    printf("new Connetion Come in %d \n", clieConnect->fd());
    printf("MainThread run EchoServer::newConnectHandler:pid = %lu \n", syscall(SYS_gettid));
    // 根据业务 redo
}

void EchoServer::closedConnectHandler(Connection *clieConnect)
{
    printf("\n");
    // 根据业务 redo
}

void EchoServer::errorConnectHandler(Connection *clieConnect)
{
    printf("new Connetion Come in %d\n", clieConnect->fd());
    // 根据业务 redo
}

/**
 * @brief 处理请求报文、在TcpServer中回调此函数
 */
void EchoServer::onMessageHandler(Connection *conn, std::string &message)
{
    printf("WorkSThread run EchoServer::onMessagehandler:pid = %lu \n", syscall(SYS_gettid));
    /* message = "reply:" + message;
    int len = message.size();

    conn->send(message.data(), message.size()); */

    // 把任务添加到线程池
    threadpool_.addTask(std::bind(&EchoServer::onMessage, this, conn, message));
}

/**
 * @brief 数据发送完成后在TcpServer回调
 */
void EchoServer::sendCompleteHandler(Connection *conn)
{
    printf("Message send complete\n");
    // 根据业务 redov
}

void EchoServer::timeoutHandler(EventLoop *loop)
{
    printf("echoserver timeout \n");
    // 根据业务 redo
}
