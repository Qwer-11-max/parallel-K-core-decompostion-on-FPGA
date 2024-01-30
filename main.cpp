#include <iostream>
#include <string>
#include <vector>

#include "graphProcessFunc.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "输入参数过少，请输入要处理的文件"<<endl;
        return 0;
    }

    string fileDir(argv[1]);
    unsigned int vertex;             // 顶点数量
    unsigned int edge;               // 边数
    unsigned int *p_degrees;         // 存储顶点度数
    unsigned int *p_offset;          // 存储顶点在p_edges当中的起始位置
    std::vector<unsigned int> edges; // 存储边

    GraphProcessFunc_NVE(fileDir,vertex,edge,&p_degrees,&p_offset,edges);

    delete[] p_degrees;
    delete[] p_offset;
    return 0;
}