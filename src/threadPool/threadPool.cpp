#include "threadPool.h"

void ThreadPool::ThreadWorker::operator()()
{
    std::function<void()> func; // 定义基础函数类func
    bool dequeued;              // 是否正在取出队列中元素
    while (!mPool->mShutdown)
    {
        {
            // 为线程环境加锁，互访问工作线程的休眠和唤醒
            std::unique_lock<std::mutex> lock(mPool->mConditionalMutex);
            // 如果任务队列为空，阻塞当前线程
            if (mPool->mQueue.empty())
            {
                mPool->mConditionalLock.wait(lock); // 等待条件变量通知，开启线程
            }
            // 取出任务队列中的元素
            dequeued = mPool->mQueue.deQueue(func);
        }
        // 如果成功取出，执行工作函数
        if (dequeued)
            func();
    }
}

void ThreadPool::init()
{
    for (int i = 0; i < mThreads.size(); ++i)
    {
        mThreads.at(i) = std::thread(ThreadWorker(this, i)); // 分配工作线程
    }
}

void ThreadPool::shutdown()
{
    mShutdown = true;
    mConditionalLock.notify_all(); // 通知，唤醒所有工作线程
    for (int i = 0; i < mThreads.size(); ++i)
    {
        if (mThreads.at(i).joinable()) // 判断线程是否在等待
        {
            mThreads.at(i).join(); // 将线程加入到等待队列
        }
    }
}