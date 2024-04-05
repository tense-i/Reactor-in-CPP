#include "ThreadPool.h"
#include <sys/syscall.h>
#include <iostream>

ThreadPool::ThreadPool(size_t threadNum, const std::string &type) : stop_(false), threadType_(type)
{
    for (size_t i = 0; i < threadNum; i++)
    {
        // 用lambda创建线程
        auto threadArg = [this]()
        {
            printf("create %s thread %lu \n", this->threadType_.c_str(), syscall(SYS_gettid));
            while (stop_ == false)
            {
                std::function<void()> task; // 存放出队的元素
                {                           // 锁的作用域
                    std::unique_lock<std::mutex> lock(this->mutex_);

                    // lambda返回true时、表示条件被触发
                    // 为true表示队列非空
                    //
                    auto waitLamb = [this]
                    {
                        return ((this->stop_ == true) || (this->taskQue_.empty() == false));
                    };
                    // 等待生产者的条件变量--等待队列为空、stop_为true信号
                    this->condition_.wait(lock, waitLamb);

                    // 在线程池停止之前、如果队列还有任务、执行2完毕再退出
                    if (((this->stop_ == true) || (this->taskQue_.empty() == true))) // 没有任务
                        return;
                    // 移动语义、避免拷贝
                    task = std::move(this->taskQue_.front());
                    this->taskQue_.pop();
                } // 锁的作用域结束、自动释放锁
                printf("%s thread is %lu \n", this->threadType_.c_str(), syscall(SYS_gettid));
                task(); // 执行任务
            }
        };
        threads_.emplace_back(threadArg);
        ; // emplace_back接受参数后自动构造容器元素
    }
}

void ThreadPool::addTask(std::function<void()> task)
{
    { // 锁的作用域
        std::lock_guard<std::mutex> lock(mutex_);
        taskQue_.push(task);
        //  printf("");
    }
    condition_.notify_one(); // 发送队列不为空信号、唤醒因为队列为空而阻塞的消费者线程
}

size_t ThreadPool::size()
{
    return threads_.size();
}

ThreadPool::~ThreadPool()
{
    stop_ = true;
    condition_.notify_all(); // 诱杀

    // 等待全部线程执行完后再退出
    for (std::thread &th : threads_)
    {
        th.join();
    }
}
