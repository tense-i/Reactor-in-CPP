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
    void setAddr(sockaddr_in srcAddr);
    const char *ip() const;
    uint16_t port() const;
    const sockaddr *addr() const;
};