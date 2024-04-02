#pragma once
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"
#include "TimeStamp.h"
#include <memory>
#include <atomic>

class Connection;
using spConnection = std::shared_ptr<Connection>;

/**
 * @brief connection用于管理已连接fd的事件、处理channel的事件通知 Connection类采用智能指针管理、不适用普通指针来引用对象
 */
class Connection : public std::enable_shared_from_this<Connection>
{

private:
    std::unique_ptr<EventLoop> &evloop_;   // Acceptor对应的事件循环、构造函数中传入
    std::unique_ptr<Socket> clieSocket_;   // 用独享指针管理clieSocket_
    std::unique_ptr<Channel> clieChannel_; // 一个服务器有很多个客户端channel、不适合放在栈区
    Buffer inputBuf_;                      // 接收缓冲区
    Buffer outputBuf_;
    TimeStamp last_atime_; // 时间戳、创建COnnection对象时为当前时间、接受报文后更改当前时间

    std::atomic_bool disconnect_;

    std::function<void(spConnection)> closedCallBack_; // 关闭fd_的回调函数、将回调Tcpserver::closedconnecion
    std::function<void(spConnection)> errorCallBack_;
    std::function<void(spConnection, std::string &)> onMessageCallBack_; // 处理报文的回调函数、将回调TcpServer::onmessage();
    std::function<void(spConnection)> sendCompleteCallBack_;

public:
    Connection(std::unique_ptr<EventLoop> &loop, std::unique_ptr<Socket> clienSock);
    ~Connection();
    int fd() const;
    std::string ip() const;
    uint16_t port() const;
    void writeCallBack(); // 处理写事件、被channel回调
    void onMessage();     /*接受报文*/
    void send(const char *data, size_t size);
    void errorCallBack();
    void closedCallBack();
    void setClosedCallBack(std::function<void(spConnection)> fn);
    void setErrorCallBack(std::function<void(spConnection)> fn);
    void setOnMessageCallBack(std::function<void(spConnection, std::string &)> fn);
    void setSendCompleteCallBack(std::function<void(spConnection)> fn);
};
