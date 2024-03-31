#pragma once
#include <unistd.h>
#include <thread>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <queue>

class ThreadPool
{
private:
    std::vector<std::thread> threads_;          // 线程数组
    std::queue<std::function<void()>> taskQue_; // 任务队列
    std::mutex mutex_;                          // 互斥变量
    std::condition_variable condition_;         // 条件 变量
    std::atomic_bool stop_;                     // 析构函数中将stop_设为true、全部线程退出
    std::string threadType_;                    // 线程种类

public:
    ThreadPool(size_t threadNum, const std::string &type);
    void addTask(std::function<void()> task);
    ~ThreadPool();
};
