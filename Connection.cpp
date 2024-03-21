#include "Connection.h"

Connection::Connection(EventLoop *loop, Sock *clieSock) : loop_(loop), clieSocket_(clieSock)
{

    clienChannel_ = new Channel(loop_, clieSocket_->fd());
    clienChannel_->useET();
    // 为新客户连接准备读事件、并添加到epoll中
    clienChannel_->enableReading();
    clienChannel_->setReadCallBack(std::bind(&Channel::onMessageArvc, clienChannel_));
}

Connection::~Connection()
{
    delete clieSocket_;
    delete clienChannel_;
}
