#include "Acceptor.h"
#include "Connection.h"

Acceptor::Acceptor(std::unique_ptr<EventLoop> &evloop, const std::string &ip, const uint16_t port) : evloop_(evloop), servSocket_(createNonblockSocket()), acceptChannel_(evloop_, servSocket_.fd())
{
    // servSocket_ = new Socket(createNonblockSocket()); // Socket的析构函数会关闭fd、若在栈区创建servSocket_离开构造函数便会析构
    servSocket_.setKeepAlive(true);
    servSocket_.setReusePort(true);
    servSocket_.setRuseaddr(true);
    servSocket_.setTCPnodelay(true);
    InetAddress servAddr(ip, port);

    servSocket_.bind(servAddr);
    servSocket_.listen();

    // acceptChannel_ = new Channel(evloop_, servSocket_.fd());
    acceptChannel_.enableReading();
    acceptChannel_.setReadCallBack(std::bind(&Acceptor::newConnection, this));
}

Acceptor::~Acceptor()
{
    // 指针不好管理、用栈内存更好！！
    // delete servSocket_;
    // delete acceptChannel_;
}

void Acceptor::newConnection()
{
    InetAddress clieAddr;
    std::unique_ptr<Socket> clieSock(new Socket(servSocket_.accept(clieAddr)));
    clieSock->setInetAddr(clieAddr.ip(), clieAddr.port());
    // Connection *ClieConn = new Connection(evloop_, clieSock); // 这里new出的对象没有释放---以后再解决
    // 为什么要用移动语义？unique无法拷贝、只能用移动语义传递
    newConnectCallBack_(std::move(clieSock));
}

void Acceptor::setNewConnectCallBack(std::function<void(std::unique_ptr<Socket>)> fn)
{
    newConnectCallBack_ = fn;
}
