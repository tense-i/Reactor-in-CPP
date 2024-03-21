#include "Acceptor.h"
#include "Connection.h"

Acceptor::Acceptor(EventLoop *loop, const std::string &ip, const uint16_t port) : loop_(loop)
{
    servSocket_ = new Sock(createNonblockSock());

    InetAddress servAddr(ip, port);

    /*设置套接字属性*/
    servSocket_->setRuseaddr(true);
    servSocket_->setReusePort(true);
    servSocket_->setKeepAlive(true);
    servSocket_->setTCPnodelay(true);

    servSocket_->bind(servAddr);
    servSocket_->listen();

    acceptChannel_ = new Channel(loop_, servSocket_->fd());
    // 监听读事件
    acceptChannel_->enableReading();
    acceptChannel_->setReadCallBack(std::bind(&Acceptor::newConnect, this));
}

Acceptor::~Acceptor()
{
    delete servSocket_;
    delete acceptChannel_;
}

void Acceptor::newConnect()
{
    InetAddress clieAddr;
    Sock *clienSock = new Sock(servSocket_->accept(clieAddr));
    // 客户端的clieSock不能再栈上创建、在退栈后会自动退出、调用析构、释放fd
    printf("new client %s:%d connct...\n", clieAddr.ip(), clieAddr.port());
    newConnectCallBack_(clienSock);
}

void Acceptor::setNewConnectCallBack(std::function<void(Sock *)> fn)
{
    newConnectCallBack_ = fn;
}
