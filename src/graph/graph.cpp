#include "graph.h"
#include "fileOperations.h"

bool Graph::listInited = false;
Graph::p_GraphProcessFunc Graph::p_graphProcessFuncList[PROCESS_GRAPH_FUNC_ARRAY_SIZE] = {nullptr};
std::map<std::string, int> Graph::hash;

Graph::Graph(std::string fileDir, bool initGraph) : processFailed(false)
{
    // 加载图处理函数
    if (!listInited)
        InitGraphProcessList();
    // 判断该图是否已被处理，如果已被处理就直接读取并返回
    if (!initGraph && ParallelReadStdGraph(fileDir, vertex, edge, &p_degrees, &p_offset, edges))
        return;
    // 获取图的处理函数
    p_GraphProcessFunc func = GetGraphProcessFunc(fileDir);
    if (!func)
        return;
    // 调用图处理函数来处理图
    if (!func(fileDir, vertex, edge, &p_degrees, &p_offset, edges))
    {
        std::cout << "图处理失败" << std::endl;
        processFailed = true;
        return;
    }
    avgDeg = edge / vertex;
}

Graph::~Graph()
{
    delete[] p_degrees;
    delete[] p_offset;
}

bool Graph::ReadProcessedGraph(std::string fileDir)
{
    //======打开要读取的文件======
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
    std::ifstream fileDegrees, fileOffset, fileEdge;
    fileDegrees.open(DEGREES_FILE(fileName));
    fileOffset.open(OFFSET_FILE(fileName));
    fileEdge.open(EDGES_FILE(fileName));
    // 错误处理
    if (!fileDegrees && !fileOffset && !fileEdge)
    {
        std::cout << fileName << "未被处理过" << std::endl;
        return false;
    }

    //========读取文件======
    std::cout << fileName + "已被处理过，开始读取" << std::endl;
    fileDegrees >> vertex;
    fileEdge >> edge;
    p_degrees = new unsigned int[vertex];
    p_offset = new unsigned int[vertex];
    edges.resize(edge);

    for (int i = 0; fileDegrees; i++)
    {
        fileDegrees >> p_degrees[i];
        fileOffset >> p_offset[i];
    }
    for (int i = 0; fileEdge; i++)
    {
        fileEdge >> edges[i];
    }
    return true;
}

bool Graph::InitGraphProcessList()
{
    //====装载图处理函数======
    int i = 0;
    InsertGraphProcess(GraphProcessFunc_NVE, "nve", i++); // nve处理程序

    return true;
}