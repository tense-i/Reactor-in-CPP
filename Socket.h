#pragma once

#include "InetAddress.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

class InetAddress;
/**
 * @brief 创建非阻塞的监听套接字、不是Soket类的成员函数、是全局函数
 */
int createNonblockSocket();

class Socket
{
private:
    const int fd_;
    std::string ip_;
    uint16_t port_;

public:
    Socket();
    Socket(int fd);
    ~Socket();
    int fd() const;
    uint16_t port() const;
    std::string ip() const;

    /**
     * @brief 设置套接字选项
     */
    void setRuseaddr(bool on);   // 地址复用
    void setTCPnodelay(bool on); //
    void setKeepAlive(bool on);  //
    void setReusePort(bool on);  // 端口复用

    void bind(const InetAddress &addr);
    void listen(int queSize = 128);
    int accept(InetAddress &addr);
    void setInetAddr(const std::string &ip, uint16_t port);
};
