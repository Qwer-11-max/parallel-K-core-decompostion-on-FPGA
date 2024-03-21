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
    std::string temp;

    // 读取顶点数量
    fileIn >> temp >> vertex;
    // 设置度数和偏移数组
    degrees.resize(vertex,0);
    offsets.resize(vertex,0);

    // 读取度，偏移和边的信息
    unsigned int offset = 0, degree = 0;
    unsigned int src = 0, dst = 0, preSrc = -1;
    bool isFirst = true;

    while (fileIn >> src >> dst)
    {

        if (src != preSrc)
        {
            offsets[src] = offset;
            if (!isFirst)
            {
                degrees[preSrc] = degree;
                degree = 0;
            }
            isFirst = false;
            preSrc = src;
        }
        adjs.push_back(dst);
        offset++;
        degree++;
    }
    degrees[src] = degree;
    edge = adjs.size();
    fileIn.close();

    //开始写入文件
    std::cout << "读取完毕，开始写入文件" << std::endl;
    ParallelWriteStdGraph(fileDir,vertex,edge,degrees,offsets,adjs);
    return true;
}
