#include "InetAddress.h"

InetAddress::InetAddress()
{
}

InetAddress::~InetAddress()
{
}

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);
}

InetAddress::InetAddress(const sockaddr_in addr) : addr_(addr)
{
}

const char *InetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
    ;
}

const sockaddr *InetAddress::addr() const
{
    return (sockaddr *)&addr_;
}

void InetAddress::setAddr(sockaddr_in clieAddr)
{
    addr_ = clieAddr;
}

uint16_t InetAddress::port() const
{
    return addr_.sin_port;
}