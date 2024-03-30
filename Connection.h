#pragma once
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"

class Connection
{
private:
    EventLoop *evloop_; // Acceptor对应的事件循环、构造函数中传入
    Socket *clieSocket_;
    Channel *clieChannel_;
    Buffer inputBuf_; // 接收缓冲区
    Buffer outputBuf_;
    std::function<void(Connection *)> closedCallBack_; // 关闭fd_的回调函数、将回调Tcpserver::closedconnecion
    std::function<void(Connection *)> errorCallBack_;
    std::function<void(Connection *, std::string &)> onMessageCallBack_; // 处理报文的回调函数、将回调TcpServer::onmessage();
    std::function<void(Connection *)> sendCompleteCallBack_;

public:
    Connection(EventLoop *loop, Socket *clienSock);
    ~Connection();
    int fd() const;
    std::string ip() const;
    uint16_t port() const;
    void writeCallBack(); // 处理写事件、被channel回调
    void onMessage();     /*接受报文*/
    void send(const char *data, size_t size);
    void errorCallBack();
    void closedCallBack();
    void setClosedCallBack(std::function<void(Connection *)> fn);
    void setErrorCallBack(std::function<void(Connection *)> fn);
    void setOnMessageCallBack(std::function<void(Connection *, std::string &)> fn);
    void setSendCompleteCallBack(std::function<void(Connection *)> fn);
};
