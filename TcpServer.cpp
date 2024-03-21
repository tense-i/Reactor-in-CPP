#include "TcpServer.h"
#include "Connection.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->setNewConnectCallBack(std::bind(&TcpServer::newConnect, this, std::placeholders::_1)); //_1是预留位置、因为  现在没有适合的ClienSock;
}

TcpServer::~TcpServer()
{
    delete acceptor_;
}

void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newConnect(Sock *clieSock)
{
    Connection *clieConnect = new Connection(&loop_, clieSock); // 还是没有释放！！！
}
