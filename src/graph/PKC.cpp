#include <queue>
#include <mutex>
#include <atomic>
#include <fstream>
#include <shared_mutex>

#include "graph.h"
#include "threadPool.h"
#include "configure.h"

/// @file PKC.cpp
/// @attention 使用循环剥离的方法来对图进行分解，循环剥离即在每一次迭代中剥离度数为k的顶点，直到没有被剥离的顶点为止，进入下一个循环

/// @brief 将度数小于k的顶点放到waitForProcess数组中等待loop的处理
/// @param degrees 顶点的度数集
/// @param processed 判断数组是否被处理过
/// @param k k-core中的k
/// @param waitQueue 等待处理的顶点队列
/// @param id 进程id号
/// @param hardwareCount 线程总数
int ScanGraph(std::vector<SafeQueue<unsigned int>> &waitQueue,
              std::vector<std::atomic_uint> &degrees, std::vector<std::atomic_bool> &processed, int k, unsigned int id, int hardwareCount)
{
    //======逐个扫描顶点的度数，将度<=k的顶点放入待处理队列=======
    unsigned int n = degrees.size();
    for (unsigned int i = 0; i < n; i += hardwareCount)
    {
        unsigned int loc = id + i;
        if (loc < n && !processed[loc].load() && degrees[loc].load() <= k)
            waitQueue[id].enQueue(loc);
    }
    return 1;
}

/// @brief 处理等待队列中的顶点，处理后将processed[]中相应位置置为true
/// @param coreVertexs 存储处理后顶点的数组
/// @param k k-core中的k
/// @param id 线程id
/// @param waitVector 等待队列
/// @param processed 被处理标志
/// @param atomicDegrees 能够原子操作的顶点度数组
/// @param offsets 顶点的第一个邻接点在adjs中的位置
/// @param adjs 压缩后的邻接点数组
/// @return 该函数处理的顶点数
int loopGraph(SafeQueue<unsigned int> &coreVertexs,
              int k, int id, std::vector<SafeQueue<unsigned int>> &waitVector, std::vector<std::atomic_bool> &processed,
              std::vector<std::atomic_uint> &atomicDegrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs)
{
    //==========扫描各个等待队列，将符合条件的邻接点加入处理过的队列========
    int count = 0;

    unsigned int num = 0;
    while (!waitVector[id].empty())
    {
        waitVector[id].deQueue(num);
        coreVertexs.enQueue(num);
        processed[num].store(true);
        for (unsigned int j = offsets[num]; j < offsets[num + 1]; j++)
        {
            // 邻接点未被处理过，且度数大于k，则其度数减一
            if (!processed[adjs[j]].load() && atomicDegrees[adjs[j]].load() > k)
            {
                unsigned int deg_u = atomicDegrees[adjs[j]].fetch_sub(1);
                if (deg_u == k + 1)
                    waitVector[id].enQueue(adjs[j]);
            }
        }
        count++;
    }

    return count;
}

bool Graph::PKC()
{
    //=========进行k核分解前的先期准备========
    std::vector<std::atomic_bool> processed(vertex); // 标记顶点是否被处理过，如果被处理过则置为true，否则false
    for (auto &x : processed)
    {
        x.store(false);
    }
    int hardwareCount = std::thread::hardware_concurrency();       // 查询硬件支持的线程数量
    std::vector<SafeQueue<unsigned int>> waitQueue(hardwareCount); // 等待处理的顶点队列，每个线程一个
    std::vector<std::atomic_uint> atomicDegrees(vertex);           // 将degrees数组转换为可以原子操作的数组，方便多线程操作
    // 初始化原子变量
    for (int i = 0; i < vertex; i++)
    {
        atomicDegrees[i].store(degrees[i]);
    }

    std::vector<unsigned int> coreOffsets; // k阶核的起始位置
    SafeQueue<unsigned int> coreVertex;    // 存储核中的点
    int k = 0; //当前处理的k核
    unsigned int count = 0; //处理过的顶点数量
    //=========并行进行k核分解===============
    ThreadPool pool(hardwareCount); // 创建一个有hardware_concurrency个工作线程的线程池
    pool.init();

    while (count < vertex)
    {
        coreOffsets.emplace_back(coreVertex.size());
        std::vector<std::future<int>> scan;
        for (int i = 0; i < hardwareCount; i++)
        {
            scan.emplace_back(pool.submit(ScanGraph, std::ref(waitQueue), std::ref(atomicDegrees), std::ref(processed), k, i, hardwareCount));
        }

        for (auto &x : scan)
        {
            x.get();
        }

        std::vector<std::future<int>> loop;
        for (int i = 0; i < hardwareCount; i++)
        {
            loop.emplace_back(pool.submit(loopGraph, std::ref(coreVertex), k, i,
                                          std::ref(waitQueue), std::ref(processed), std::ref(atomicDegrees),
                                          std::ref(offsets), std::ref(adjs)));
        }
        for (auto &x : loop)
        {
            count += x.get();
        }
        k++;
    }
    pool.shutdown();
    //=========k核分解完成，进行其他操作=====

    // 将结果写入文件
    std::ofstream fileOffsets, fileVertex;

    // 写入偏移
    fileOffsets.open(OUTPUT_FILE(fileName + "_Offset"));
    for (auto &i : coreOffsets)
        fileOffsets << i << std::endl;
    fileOffsets.close();

    // 写入点
    fileVertex.open(OUTPUT_FILE(fileName + "_Vertex"));
    for (unsigned int i = 0, temp; i < vertex; i++)
    {
        coreVertex.deQueue(temp);
        fileVertex << temp << std::endl;
    }
    return false;
}