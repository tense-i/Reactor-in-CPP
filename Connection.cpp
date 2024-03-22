#include "Connection.h"

Connection::Connection(EventLoop *loop, Sock *clieSock) : loop_(loop), clieSocket_(clieSock)
{

    clienChannel_ = new Channel(loop_, clieSocket_->fd());
    clienChannel_->setClosedCallBack(std::bind(&Connection::closedCallBack, this));
    clienChannel_->setErrorCallBack(std::bind(&Connection::errorCallBack, this));
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

int Connection::fd() const
{
    return clieSocket_->fd();
}

std::string Connection::ip() const
{
    return clieSocket_->ip();
}

uint16_t Connection::port() const
{
    return clieSocket_->port();
}

void Connection::closedCallBack()
{
    closeCallBack_(this);
}

void Connection::errorCallBack()
{
    errorCallBack_(this);
}

void Connection::setClosedCallBack(std::function<void(Connection *)> fn)
{
    closeCallBack_ = fn;
}

void Connection::setErrorCallBack(std::function<void(Connection *)> fn)
{
    errorCallBack_ = fn;
}
