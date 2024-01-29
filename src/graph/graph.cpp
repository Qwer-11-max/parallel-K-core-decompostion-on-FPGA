#include "graph.h"

Graph::Graph(std::string fileDir)
{
}

Graph::~Graph()
{
    delete[] p_degrees;
    delete[] p_edges;
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
