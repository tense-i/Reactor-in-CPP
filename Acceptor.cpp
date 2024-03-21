#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *loop, const std::string &ip, const uint16_t port) : loop_(loop)
{
    servSocket_ = new Sock(createNonblockSock());

    InetAddress servAddr(ip, port);

    servSocket_->setRuseaddr(true);
    servSocket_->setReusePort(true);
    servSocket_->setKeepAlive(true);
    servSocket_->setTCPnodelay(true);

    servSocket_->bind(servAddr);
    servSocket_->listen();

    acceptChannel_ = new Channel(loop_->ep(), servSocket_->fd());
    // 监听读事件
    acceptChannel_->enableReading();
    acceptChannel_->setReadCallBack(std::bind(&Channel::newConnect, acceptChannel_, servSocket_));
}

Acceptor::~Acceptor()
{
    delete servSocket_;
    delete acceptChannel_;
}
