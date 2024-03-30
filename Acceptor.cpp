#include "Acceptor.h"
#include "Connection.h"

Acceptor::Acceptor(EventLoop *evloop, const std::string &ip, const uint16_t port) : evloop_(evloop)
{
    servSocket_ = new Socket(createNonblockSocket()); // Socket的析构函数会关闭fd、若在栈区创建servSocket_离开构造函数便会析构
    servSocket_->setKeepAlive(true);
    servSocket_->setReusePort(true);
    servSocket_->setRuseaddr(true);
    servSocket_->setTCPnodelay(true);
    InetAddress servAddr(ip, port);

    servSocket_->bind(servAddr);
    servSocket_->listen();

    acceptChannel_ = new Channel(evloop_, servSocket_->fd());
    acceptChannel_->enableReading();
    acceptChannel_->setReadCallBack(std::bind(&Acceptor::newConnection, this));
}

Acceptor::~Acceptor()
{
    delete servSocket_;
    delete acceptChannel_;
}

void Acceptor::newConnection()
{
    InetAddress clieAddr;
    Socket *clieSock = new Socket(servSocket_->accept(clieAddr)); // 为啥是new?不用局部对象

    // Connection *ClieConn = new Connection(evloop_, clieSock); // 这里new出的对象没有释放---以后再解决
    newConnectCallBack_(clieSock);
}

void Acceptor::setNewConnectCallBack(std::function<void(Socket *)> fn)
{
    newConnectCallBack_ = fn;
}
