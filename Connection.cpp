#include "Connection.h"

Connection::Connection(std::unique_ptr<EventLoop> &evloop, std::unique_ptr<Socket> clienSock) : evloop_(evloop), clieSocket_(std::move(clienSock)), disconnect_(false), clieChannel_(new Channel(evloop_, clieSocket_->fd())) // uniqueptr是无法拷贝的--确保只有一份指针、要用移动语义！！
{

    // ep.addFd(clieSock->fd(), (EPOLLIN | EPOLLET)); // 客户端连上的fd采用边缘触发

    clieChannel_->setReadCallBack(std::bind(&Connection::onMessage, this));
    clieChannel_->setClosedCallBack(std::bind(&Connection::closedCallBack, this));
    clieChannel_->setErrorCallBack(std::bind(&Connection::errorCallBack, this));
    clieChannel_->setWriteCallBack(std::bind(&Connection::writeCallBack, this));

    clieChannel_->enableReading();
    clieChannel_->useET();
}

Connection::~Connection()
{
    // unique自动释放
    //  delete clieSocket_;
    // delete clieChannel_;
    printf("Connection 析构....\n");
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
    printf("server write fd:%s \n ", outputBuf_.data());
    if (nwrite > 0)
    {
        outputBuf_.eraseDate(0, nwrite);
    }

    if (outputBuf_.size() == 0) // 若缓冲区没有数据可写了、表示数据发送成功、不再关注写事件
    {
        clieChannel_->disableWrite();
        sendCompleteCallBack_(shared_from_this());
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
                printf("server recv %s \n", message.c_str());
                inputBuf_.eraseDate(0, len + 4);
                // last_atime_ = TimeStamp::now();
                // std::cout << "last_atime = " << last_atime_.toString() << std::endl;
                onMessageCallBack_(shared_from_this(), message);
            };
            break;
        }
        else if (nread == 0) // 客户端半关闭
        {
            // closedCallBack_();
            // clieChannel_->remove(); // 删除已关闭fd的channel管理器
            closedCallBack();
            break;
        }
    }
}

void Connection::send(const char *data, size_t size)
{
    if (disconnect_ == true)
    {
        printf("客户端已经断开连接、不在发送数据过去\n");
        return;
    }
    outputBuf_.appendWithHead(data, size); // 把需要发送的数据保存在connection的发送缓冲区中。
    // 注册写事件
    clieChannel_->enableWrite(); // 监视写事件
}

void Connection::errorCallBack()
{
    /* disconnect_ = true;

    printf("client(evnetfd = %d) error\n", fd());
    close(fd()); */
    disconnect_ = true;
    clieChannel_->remove();
    errorCallBack_(shared_from_this());
}

void Connection::closedCallBack()
{
    /* printf("client(evnetfd = %d) disconnection\n", fd());
    disconnect_ = true;
    clieChannel_->remove();
    close(fd()); */
    disconnect_ = true;
    clieChannel_->remove();
    closedCallBack_(shared_from_this());
}

void Connection::setClosedCallBack(std::function<void(spConnection)> fn)
{
    closedCallBack_ = fn;
}

void Connection::setErrorCallBack(std::function<void(spConnection)> fn)
{
    errorCallBack_ = fn;
}

void Connection::setOnMessageCallBack(std::function<void(spConnection, std::string &)> fn)
{
    onMessageCallBack_ = fn;
}

void Connection::setSendCompleteCallBack(std::function<void(spConnection)> fn)
{
    sendCompleteCallBack_ = fn;
}
