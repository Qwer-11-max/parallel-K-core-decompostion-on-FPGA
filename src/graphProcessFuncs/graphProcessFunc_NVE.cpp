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

    unsigned int maxCount = 0;        // 图的最大顶点id + 1
    unsigned int src, dst;            // 一条边
    unsigned int degree = 0, cur = 0; // 记录度数,邻接数组的大小
    unsigned int preSrc = 0;          // 记录前一个起始点
    bool start = true;                // 表明图刚开始

    while (fileIn >> src >> dst) // 每读取一条边
    {
        unsigned int a = max(src, dst) + 1;
        if (a > maxCount) // 更新顶点最大id
        {
            degrees.resize(a,0);
            offsets.resize(a,0);
            maxCount = a;
        }

        // 将邻接点压入邻接数组
        adjs.emplace_back(dst);
        // 判断是否要存储度
        if (start)
        {
            preSrc = src;
            start = false;
        }
        else if (preSrc != src)
        {
            degrees[preSrc] = degree;
            offsets[preSrc] = cur - degree;
            preSrc = src;
            degree = 0;
        }
        // 度数加一
        degree++;
        cur++;
    }
    degrees[preSrc] = degree;
    offsets[preSrc] = cur - degree;
    vertex = maxCount;
    edge = cur;
    // 关闭文件
    fileIn.close();

    // 开始写入文件
    std::cout << "读取完毕，开始写入文件" << std::endl;
    ParallelWriteStdGraph(fileDir, vertex, edge, degrees, offsets, adjs);
    return true;
}
