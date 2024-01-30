#include "graph.h"

Graph::Graph(std::string fileDir)
{
    // 加载图处理函数
    if(!listInited) InitGraphProcessList();
    // 判断该图是否已被处理，如果已被处理就直接读取并返回
    if(ReadProcessedGraph) return;
    // 获取图的处理函数
    p_GraphProcessFunc func;
    // 调用图处理函数来处理图
    func(fileDir,vertex,edge,&p_degrees,&p_offset,edges);
}

Graph::~Graph()
{
    delete[] p_degrees;
    delete[] p_offset;
}

bool Graph::InsertGraphProcess(p_GraphProcessFunc func)
{
    for (int i = 0; i < PROCESS_GRAPH_FUNC_ARRAY_SIZE; i++)
    {
        if (!p_processPrimitiveGraphLists[i])
        {
            p_processPrimitiveGraphLists[i] = func;
            return true;
        }
    }
    return false;
}

bool Graph::ReadProcessedGraph()
{

}


bool Graph::InitGraphProcessList(){
    InsertGraphProcess(GraphProcessFunc_NVE); //nve处理程序
}