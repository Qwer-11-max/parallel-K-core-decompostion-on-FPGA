#include "graph.h"
#include "graphProcessFuncs.h"
#include "parallelRWFile.h"

bool Graph::listInited = false;
Graph::p_GraphProcessFunc Graph::p_graphProcessFuncList[PROCESS_GRAPH_FUNC_ARRAY_SIZE] = {nullptr};
std::map<std::string, int> Graph::hash;

Graph::Graph(std::string fileDir, bool initGraph) : processFailed(false)
{
    // 加载图处理函数
    if (!listInited)
        InitGraphProcessList();
    // 判断该图是否已被处理，如果已被处理就直接读取并返回
    if (!initGraph && ParallelReadStdGraph(fileDir, vertex, edge, degrees, offsets, adjs))
        return;
    // 获取图的处理函数
    p_GraphProcessFunc func = GetGraphProcessFunc(fileDir);
    if (!func)
        return;
    // 调用图处理函数来处理图
    if (!func(fileDir, vertex, edge, degrees, offsets, adjs))
    {
        std::cout << "图处理失败" << std::endl;
        processFailed = true;
        return;
    }
    avgDeg = edge / vertex;
}

Graph::~Graph()
{
}

bool Graph::InitGraphProcessList()
{
    //====装载图处理函数======
    int i = 0;
    InsertGraphProcess(GraphProcessFunc_NVE, "nve", i++); // nve处理程序

    return true;
}