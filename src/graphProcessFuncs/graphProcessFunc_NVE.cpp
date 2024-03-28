#include <set>

#include "graphProcessFuncs.h"

bool GraphProcessFunc_NVE(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                          std::vector<unsigned int> &degrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs)
{
    std::ifstream fileIn;
    fileIn.open(fileDir); // 打开要处理的数据集

    //======文件错误处理=======
    if (!fileIn)
    {
        std::cout << "文件错误，无法打开！！" << std::endl;
        return false;
    }

    /*===处理图===*/
    std::cout << "开始处理nve文件" << std::endl;
    // 读取第一行并弃用
    std::string firstLine;
    getline(fileIn, firstLine);

    std::vector<std::pair<unsigned int, unsigned int>> lines;
    unsigned int maxCount = 0; // 图的最大顶点id + 1
    unsigned int src, dst;     // 一条边

    while (fileIn >> src >> dst) // 每读取一条边
    {
        if (src == dst) //跳过指向自己的边
            continue;
        maxCount = max(src, maxCount);
        maxCount = max(dst, maxCount); //更新maxCount
        lines.push_back({src, dst});
    }
    // 关闭文件
    fileIn.close();

    maxCount++; //表明有多少个顶点
    std::vector<std::set<unsigned int>> vertexs(maxCount); //用于将有向图转换为无向图

    for (auto &x : lines) //用set将有向图转换为无向图
    {
        vertexs[x.first].insert(x.second);
        vertexs[x.second].insert(x.first);
    }

    vertex = maxCount;
    degrees.resize(maxCount);
    for (int i = 0; i < maxCount; i++) //记录顶点度
    {
        degrees[i] = vertexs[i].size();
    }

    offsets.resize(maxCount + 1);
    offsets[0] = 0;
    for (int i = 0; i < maxCount; i++) //记录顶点的偏移
    {
        offsets[i + 1] = offsets[i] + degrees[i];
    }

    edge = offsets[maxCount];
    adjs.resize(edge);
#pragma omp parallel for
    for (int i = 0; i < maxCount; i++) //记录顶点的邻接点
    {
        auto iter = vertexs[i].begin();
        for (int j = offsets[i]; j < offsets[i + 1]; j++, iter++)
            adjs[j] = *iter;
    }
    // 开始写入文件
    std::cout << "读取完毕，开始写入文件" << std::endl;
    ParallelWriteStdGraph(fileDir, vertex, edge, degrees, offsets, adjs);
    return true;
}
