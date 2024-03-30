#include "EchoServer.h"

EchoServer::EchoServer(const std::string &ip, const uint16_t port) : tcpserv_(ip, port)
{
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

//
void EchoServer::newConnectHandler(Connection *clieConnect)
{
    printf("new Connetion Come in\n");
    // 根据业务 redo
}

void EchoServer::closedConnectHandler(Connection *clieConnect)
{
    printf("\n");
    // 根据业务 redo
}

void EchoServer::errorConnectHandler(Connection *clieConnect)
{
    printf("new Connetion Come in\n");
    // 根据业务 redo
}

/**
 * @brief 处理请求报文、在TcpServer中回调此函数
 */
void EchoServer::onMessageHandler(Connection *conn, std::string &message)
{
    message = "reply:" + message;
    int len = message.size();
    // printf("TcpServ::onmsg :message %s\n", message.data()); 由于 std::string 类型的 buf_ 对象可能包含空字符，所以在使用 printf 函数打印时，可能会出现问题，因为 printf 函数会将空字符视为字符串的结束标志。这可能导致 printf 函数只打印 buf_ 中第一个空字符之前的内容

    // 创建一个临时缓冲区，用于存储消息长度
    // std::string tmpBuf((char *)&len, 4); // 将消息长度写进tmpBuf
    // 将实际内容添加到tmpBuf
    // tmpBuf.append(message);
    // 发送消息
    conn->send(message.data(), message.size());
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
