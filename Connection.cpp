#include "Connection.h"

Connection::Connection(EventLoop *evloop, Socket *clienSock) : evloop_(evloop), clieSocket_(clienSock)
{
    clieChannel_ = new Channel(evloop_, clieSocket_->fd());
    clieChannel_->enableReading();
    clieChannel_->useET();
    // ep.addFd(clieSock->fd(), (EPOLLIN | EPOLLET)); // 客户端连上的fd采用边缘触发

    clieChannel_->setReadCallBack(std::bind(&Connection::onMessage, this));
    clieChannel_->setClosedCallBack(std::bind(&Connection::closedCallBack, this));
    clieChannel_->setErrorCallBack(std::bind(&Connection::errorCallBack, this));
    clieChannel_->setWriteCallBack(std::bind(&Connection::writeCallBack, this));
}

Connection::~Connection()
{
    delete clieSocket_;
    delete clieChannel_;
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

void Connection::writeCallBack()
{
    // 从outputBuf里将数据写入到fd
    int nwrite = ::send(fd(), outputBuf_.data(), outputBuf_.size(), 0);
    // printf("server write fd:%s \n ", outputBuf_.data());
    if (nwrite > 0)
    {
        outputBuf_.eraseDate(0, nwrite);
    }

    if (outputBuf_.size() == 0) // 若缓冲区没有数据可写了、表示数据发送成功、不再关注写事件
    {
        clieChannel_->disableWrite();
        sendCompleteCallBack_(this);
    }
}

void Connection::onMessage()
{
    char buf[1024];
    while (true)
    {
        memset(buf, 0, sizeof(buf));
        int nread = read(fd(), buf, sizeof(buf));
        if (nread > 0)
        {
            // printf("server(event %d): %s\n", fd(), buf);
            // write(fd(), buf, nread);
            inputBuf_.append(buf, nread);

        } // 信号中断
        else if (nread == -1 && (errno == EINTR))
        {

            continue;
        } // 数据读取完毕
        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            // printf("server recv(event = %d) :%s\n", fd(), inputBuf_.data());

            // 一个一个分割报文
            while (true)
            {
                int len = 0;
                memcpy(&len, inputBuf_.data(), 4); // 将报文首部从报文中提取出来
                // 接受缓存区的大小小于len+4(len为一个完整数据报文的大小、加上4就是整个报文的大小)
                if (inputBuf_.size() < len + 4)
                    break; // 即这个报文不完整、不直接读取到发生缓冲区、等待后续数据
                // 执行到这、说明是一个完整报文
                std::string message(inputBuf_.data() + 4, len);
                inputBuf_.eraseDate(0, len + 4); // 在接受(input)删除报文
                                                 // printf("server recv(event = %d) :%s\n", fd(), message.data());
                //  在这经过复杂运算

                /*  message = "reply:" + message;
                 len = message.size();
                 std::string tmpBuf((char *)&len, 4); // 将头部写进tmpBUf
                 tmpBuf.append(message.data(), len);
                 send(fd(), tmpBuf.data(), tmpBuf.size(), 0);
                  */
                onMessageCallBack_(this, message);
            };
            break;
        }
        else if (nread == 0) // 客户端半关闭
        {
            // closedCallBack_();
            closedCallBack();
        }
    }
}

void Connection::send(const char *data, size_t size)
{
    outputBuf_.appendWithHead(data, size); // 把需要发送的数据保存在connection的发送缓冲区中。
    // 注册写事件
    clieChannel_->enableWrite(); // 监视写事件
}

void Connection::errorCallBack()
{
    printf("client(evnetfd = %d) error\n", fd());
    close(fd());
}

void Connection::closedCallBack()
{
    printf("client(evnetfd = %d) disconnection\n", fd());
    close(fd());
}

void Connection::setClosedCallBack(std::function<void(Connection *)> fn)
{
    closedCallBack_ = fn;
}

void Connection::setErrorCallBack(std::function<void(Connection *)> fn)
{
    errorCallBack_ = fn;
}

void Connection::setOnMessageCallBack(std::function<void(Connection *, std::string &)> fn)
{
    onMessageCallBack_ = fn;
}

void Connection::setSendCompleteCallBack(std::function<void(Connection *)> fn)
{
    sendCompleteCallBack_ = fn;
}
