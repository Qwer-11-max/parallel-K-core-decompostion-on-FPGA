#pragma once

#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <condition_variable>

template <class T>
class SafeQueue
{
private:
    std::mutex mMutex;    // 互斥量，用于控制队列访问
    std::queue<T> mQueue; // 队列
public:
    SafeQueue(){};
    SafeQueue(SafeQueue &&other){};
    ~SafeQueue(){};

    /// @brief 返回队列是否为空
    /// @return 为空返回True，否则返回False
    inline bool empty()
    {
        // 给队列上锁，当函数结束后自动解锁
        std::unique_lock<std::mutex> lock(mMutex);

        return mQueue.empty();
    }

    /// @brief 获取队列的长度
    /// @return 队列长度
    inline int size()
    {
        std::unique_lock<std::mutex> lock(mMutex);

        return mQueue.size();
    }

    /// @brief 向队列中插入元素
    /// @param t 要插入的元素
    inline void enQueue(T &t)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.emplace(t);
    }

    /// @brief 从队列中取任务
    /// @param t 从队列中取得的任务
    /// @return  成功找到任务则返回True，否则返回false
    inline bool deQueue(T &t)
    {
        std::unique_lock<std::mutex> lock(mMutex);

        if (mQueue.empty())
            return false;

        t = std::move(mQueue.front()); // 返回值

        mQueue.pop(); // 弹出队列中的第一个元素
        return true;
    }
};

class ThreadPool
{
private:
    class ThreadWorker // 内置线程工作类
    {
    private:
        int mId;           // 工作id
        ThreadPool *mPool; // 所属线程池
    public:
        /// @brief 工作线程，用于执行线程池中的任务
        /// @param pool 所属的线程池
        /// @param id 该工作线程的id
        ThreadWorker(ThreadPool *pool, const int id) : mPool(pool), mId(id) {}
        // 重载()操作
        void operator()();
    };
    bool mShutdown;                           // 线程池是否关闭
    SafeQueue<std::function<void()>> mQueue;  // 执行函数安全队列，即任务队列
    std::vector<std::thread> mThreads;        // 工作线程队列
    std::mutex mConditionalMutex;             // 线程休眠锁互斥变量
    std::condition_variable mConditionalLock; // 线程环境锁，可以让线程处于休眠或者唤醒状态
public:
    // 线程池构造函数
    ThreadPool(const int n_threads = 4)
        : mThreads(std::vector<std::thread>(n_threads)), mShutdown(false){};
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;
    /// @brief 初始化线程池，创建若干个(n)工作线程
    void init();
    /// @brief 通知所有子线程，等待所有子线程执行完毕
    void shutdown();

    /// @brief 向线程池提交任务，存储到任务队列中，工作线程从任务队列中获取
    /// @tparam F 要执行的函数类型
    /// @tparam ...Args 要传递的参数
    /// @param f 要执行的函数
    /// @param ...args 传递参数列表
    /// @return 返回任务的future，用于获取任务的执行结果
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        // 组合函数和其参数，便于之后的运行
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误
        // 将其封装到共享指针中，以便能够复制构造
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        // 将函数包装至一个空函数中
        std::function<void()> warpper_func = [task_ptr]()
        {
            (*task_ptr)();
        };
        // 队列通用安全封包函数，并压入安全队列
        mQueue.enQueue(warpper_func);
        // 唤醒一个等待中的线程
        mConditionalLock.notify_one();
        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
};