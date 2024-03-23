#include "Connection.h"

Connection::Connection(EventLoop *loop, Sock *clieSock) : loop_(loop), clieSocket_(clieSock)
{

    clienChannel_ = new Channel(loop_, clieSocket_->fd());
    clienChannel_->setClosedCallBack(std::bind(&Connection::closedCallBack, this));
    clienChannel_->setErrorCallBack(std::bind(&Connection::errorCallBack, this));
    clienChannel_->setWriteCallBack(std::bind(&Connection::writeCallBack, this));
    clienChannel_->useET();
    // 为新客户连接准备读事件、并添加到epoll中
    clienChannel_->enableReading();
    clienChannel_->setReadCallBack(std::bind(&Connection::onMessageArrive, this));
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

void Connection::onMessageArrive()
{
    char buf[1024];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t nread = read(fd(), buf, sizeof(buf));
        if (nread > 0)
        {
            // printf("recv(event=%d):%s\n", fd(), buf);
            // send(fd(), buf, strlen(buf), 0);
            inputBuf.append(buf, nread);
        } // 读取数据时被中断、继续读取
        else if (nread == -1 && errno == EINTR)
        {
            continue;
        } // 全部数据已经读取到inputbuf中
        else if (nread == -1 && ((errno == EAGAIN)) || (errno == EWOULDBLOCK))
        {
            printf("recv(eventfd =%d):%s\n", fd(), inputBuf.data());
            int len = 0;
            memcpy(&len, inputBuf.data(), 4); // 将报文首部从报文中提取出来
            // 接受缓存区的大小小于len+4(len为一个完整数据报文的大小、加上4就是整个报文的大小)
            if (inputBuf.size() < len + 4)
                break; // 即这个报文不完整、不直接读取到发生缓冲区、等待后续数据
            // 执行到这、说明是一个完整报文
            std::string message(inputBuf.data() + 4, len);
            inputBuf.eraseDate(0, len + 4); // 在接受(input)删除报文
            printf("message (eventfd = %d): %s \n", fd(), message.c_str());
            onMessageCallBack_(this, message);
            break;
        }
        else if (nread == 0)
        {
            closedCallBack();
            break;
        }
    }
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

void Connection::setMessageCallBack(std::function<void(Connection *, std::string)> fn)
{
    onMessageCallBack_ = fn;
}

void Connection::send(const char *data, size_t size)
{
    outputBuf.append(data, size); // 把需要发送的数据保存在connection的发送缓冲区中。
    // 注册写事件
    clienChannel_->enableWrite(); // 监视写事件
}

void Connection::writeCallBack()
{
    int nwrite = ::send(fd(), outputBuf.data(), outputBuf.size(), 0);
    if (nwrite > 0)
    {
        outputBuf.eraseDate(0, nwrite);
    }

    if (outputBuf.size() == 0) // 若缓冲区没有数据了、表示数据发送成功、不再关注写事件
    {
        clienChannel_->disableWrite();
        sendCompleteCallBack_(this);
    }
}

void Connection::setSendCompleteCallBack(std::function<void(Connection *)> fn)
{
    sendCompleteCallBack_ = fn;
}
