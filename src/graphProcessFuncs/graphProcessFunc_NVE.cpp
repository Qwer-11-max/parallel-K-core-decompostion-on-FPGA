#include <graphProcessFunc.h>

bool GraphProcessFunc_NVE(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                          unsigned int **p_degrees, unsigned int **p_offset, std::vector<unsigned int> &edges)
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
    *p_degrees = new unsigned int[vertex];
    *p_offset = new unsigned int[vertex];

    // 读取度，偏移和边的信息
    unsigned int offset = 0, degree = 0;
    unsigned int src = 0, dst = 0, preSrc = -1;
    bool isFirst = true;
    while (fileIn)
    {
        fileIn >> src >> dst;

        if (src != preSrc)
        {
            (*p_offset)[src] = offset;
            if (!isFirst)
            {
                (*p_degrees)[preSrc] = degree;
                degree = 0;
            }
            isFirst = false;
            preSrc = src;
        }
        edges.push_back(dst);
        offset++;
        degree++;
    }
    (*p_degrees)[src] = degree;
    edge = edges.size();
    fileIn.close();
    std::cout << "读取完毕，开始写入文件" << std::endl;
    /*=====将处理后的图分为三个文件存储，有助于之后的并行读取======*/
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
    std::ofstream fileDegrees, fileOffset, fileEdge;
    fileDegrees.open(PROCESSED_GRAPH_DIR + fileName + "_Degrees.txt");
    fileOffset.open(PROCESSED_GRAPH_DIR + fileName + "_Offset.txt");
    fileEdge.open(PROCESSED_GRAPH_DIR + fileName + "_Edges.txt");

    if (!fileEdge)
    {
        std::cout << "输出文件错误" << std::endl;
        return false;
    }
    //======写入文件======
    fileDegrees << vertex << std::endl;
    for (int i = 0; i < vertex; i++)
    {
        // std::cout << (*p_degrees)[i] << std::endl;
        fileDegrees << (*p_degrees)[i] << std::endl;
        fileOffset << (*p_offset)[i] << std::endl;
    }
    fileEdge << edge << std::endl;
    for (int i = 0; i < edge; i++)
    {
        fileEdge << edges[i] << std::endl;
    }

    std::cout << "以下文件写入完毕:" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Degrees.txt" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Offset.txt" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Edges.txt" << std::endl;
    // 关闭文件
    fileDegrees.close();
    fileOffset.close();
    fileEdge.close();
    return true;
}
