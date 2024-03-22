#include "inetaddress.h"

InetAddress::InetAddress() {}

/**
 * @brief 如果是监听fd、用这个构造函数
 */
InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    this->addr_.sin_family = AF_INET;
    this->addr_.sin_port = htons(port);
    this->addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

/**
 * @brief 如果是客户端的已连接套接字、用这个
 */
InetAddress::InetAddress(const sockaddr_in addr) : addr_(addr)
{
}

/**
 * @brief
 */
const char *InetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddress::port() const
{
    return this->addr_.sin_port;
}

const sockaddr *InetAddress::addr() const
{
    return (sockaddr *)&addr_;
}

void InetAddress::setAddr(sockaddr_in srcAddr)
{
    this->addr_ = srcAddr;
}

InetAddress::~InetAddress() {}
