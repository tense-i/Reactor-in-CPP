#include "TcpServer.h"
#include "Connection.h"
#include <functional>
#include <utility>

TcpServer::TcpServer(const std::string &ip, uint16_t port, int threadNum) : threadNum_(threadNum)
{
    mainLoop_ = new EventLoop;
    acceptor_ = new Acceptor(mainLoop_, ip, port);
    threadpool_ = new ThreadPool(threadNum_, "IO"); // 创建线程池
    // 创建从事件循环

    for (int i = 0; i < threadNum_; i++)
    {
        subLoops_.push_back(new EventLoop); // 创建从事件循坏
        subLoops_[i]->setEpollTimeoutCallBack(std::bind(&TcpServer::epollTimeout, this, std::placeholders::_1));
        // 绑定run任务
        threadpool_->addTask(std::bind(&EventLoop::run, subLoops_[i]));
    }
    acceptor_->setNewConnectCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));

    mainLoop_->setEpollTimeoutCallBack(std::bind(&TcpServer::epollTimeout, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
    delete acceptor_;
    delete mainLoop_;
    // 释放所有的客户端connection
    // 有个问题当连接失败时、怎么释放对应的channel
    for (auto &conn : connects_)
    {
        delete conn.second;
    }
}

void TcpServer::start()
{
    mainLoop_->run();
}

void TcpServer::sendComplete(Connection *conn)
{
    // EchoServ业务
    sendCompleteCB_(conn);

    // printf("send complete\n");
    //  根据业务 redo
}

void TcpServer::epollTimeout(EventLoop *loop)
{
    // EchoServer业务
    epollTimeoutCB_(loop);

    // printf("epoll timeout\n");
    // 根据业务 redo
}

void TcpServer::newConnection(Socket *clieSock)
{
    // Connection *clieConnect = new Connection(mainLoop_, clieSock);
    // 将新建的conn分配给从事件循环
    Connection *clieConnect = new Connection(subLoops_[clieSock->fd() % threadNum_], clieSock);

    //  存储连接客户端的channel
    connects_.insert(std::make_pair(clieConnect->fd(), clieConnect));

    clieConnect->setClosedCallBack(std::bind(&TcpServer::closedConnection, this, std::placeholders::_1));
    clieConnect->setErrorCallBack(std::bind(&TcpServer::errorConnection, this, std::placeholders::_1));
    clieConnect->setSendCompleteCallBack(std::bind(&TcpServer::sendComplete, this, std::placeholders::_1));
    clieConnect->setOnMessageCallBack(std::bind(&TcpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));

    // 回调EchoServer::newConnectin函数、处理业务
    newConnectCB_(clieConnect);

    // printf("new client(fd=%d address %s:%d)\n", clieSock->fd(), clieConnect->ip().data(), clieConnect->port()); // 日志应该再上层类记录
}

void TcpServer::closedConnection(Connection *clieConnect)
{
    closedConnectionCB_(clieConnect); // 为什么要先调用EchoServer的处理业务？因为EchoServer不涉及释放内存

    // printf("client(evnetfd = %d) disconnetion \n", clieConnect->fd());
    //  close(clieConnect->fd());为什么不用释放？
    //  clieconnetc中含有socket而socket中释放了fd
    connects_.erase(clieConnect->fd());
    delete clieConnect;
}

void TcpServer::errorConnection(Connection *clieConnect)
{
    // EchoServ业务
    errorConnectionCB_(clieConnect);

    // printf("client(evnetfd = %d) error\n", clieConnect->fd());
    //  close(clieConnect->fd());
    connects_.erase(clieConnect->fd());
    delete clieConnect;
}

void TcpServer::onMessage(Connection *conn, std::string &message)
{
    /* message = "reply:" + message;
    int len = message.size();
    // printf("TcpServ::onmsg :message %s\n", message.data()); 由于 std::string 类型的 buf_ 对象可能包含空字符，所以在使用 printf 函数打印时，可能会出现问题，因为 printf 函数会将空字符视为字符串的结束标志。这可能导致 printf 函数只打印 buf_ 中第一个空字符之前的内容

    // 创建一个临时缓冲区，用于存储消息长度
    std::string tmpBuf((char *)&len, 4); // 将消息长度写进tmpBuf
    // 将实际内容添加到tmpBuf
    tmpBuf.append(message);
    // 发送消息
    conn->send(tmpBuf.data(), tmpBuf.size()); */

    // EchoServ业务
    onMessageCB_(conn, message);
}

void TcpServer::setNewConnectCB(std::function<void(Connection *)> fn)
{
    newConnectCB_ = fn;
}

void TcpServer::setClosedConnectCB(std::function<void(Connection *)> fn)
{
    closedConnectionCB_ = fn;
}

void TcpServer::setErrorConnectCB(std::function<void(Connection *)> fn)
{
    errorConnectionCB_ = fn;
}

void TcpServer::setOnMessageCB(std::function<void(Connection *, std::string &message)> fn)
{
    onMessageCB_ = fn;
}

void TcpServer::setSendCompleteCB(std::function<void(Connection *)> fn)
{
    sendCompleteCB_ = fn;
}

void TcpServer::setEpollTimeoutCB(std::function<void(EventLoop *)> fn)
{
    epollTimeoutCB_ = fn;
}
