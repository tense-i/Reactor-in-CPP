#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class InetAddress
{
private:
    sockaddr_in addr_;

public:
    InetAddress();
    ~InetAddress();
    InetAddress(const std::string &ip, uint16_t port);
    InetAddress(const sockaddr_in addr);

    void setAddr(sockaddr_in clieAddr);
    const char *ip() const;
    uint16_t port() const;

    /**
     * @brief 返回的是通用地址结构体--sockeaddr
     */
    const sockaddr *addr() const;
};