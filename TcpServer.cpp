#include "TcpServer.h"
#include "Connection.h"
#include <utility>

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->setNewConnectCallBack(std::bind(&TcpServer::newConnect, this, std::placeholders::_1)); //_1是预留位置、因为  现在没有适合的ClienSock;
}

TcpServer::~TcpServer()
{
    delete acceptor_;
    for (auto &conn : connects_)
    {
        delete conn.second;
    }
}

void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newConnect(Sock *clieSock)
{
    Connection *clieConnect = new Connection(&loop_, clieSock); // 还是没有释放！！！
    connects_[clieSock->fd()] = clieConnect;                    // 用map管理connection
    clieConnect->setClosedCallBack(std::bind(&TcpServer::closeClientConnect, this, std::placeholders::_1));
    clieConnect->setErrorCallBack(std::bind(&TcpServer::onError_closedClientConnect, this, std::placeholders::_1));
    printf("new client  %s:%d  fd= %d connct...\n", clieSock->ip().c_str(), clieSock->port(), clieSock->fd());
}

void TcpServer::closeClientConnect(Connection *clieConnect)
{
    printf("client %d :closed ...\n", clieConnect->fd());
    connects_.erase(clieConnect->fd());
}

void TcpServer::onError_closedClientConnect(Connection *clieConnect)
{
    printf("client envent=%d error\n", clieConnect->fd());
    // close(clieConnect->fd()); 是多余的、在connectin的析构函数会delete Sock而Sock的析构函数负责释放fd
    connects_.erase(clieConnect->fd());
}