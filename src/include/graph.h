#pragma once

#include <string>
#include <vector>

#include "configure.h"
#include "graphProcessFunc.h"

// 该类中要用到的一些宏
#define PROCESS_GRAPH_FUNC_ARRAY_SIZE 128 // 图处理函数列表的容量
// 用于存储图信息，包括顶点，边等信息
class Graph
{
private:
    /* 基础变量 */
    unsigned int vertex;             // 顶点数量
    unsigned int edge;               // 边数
    unsigned int *p_degrees;         // 存储顶点度数
    unsigned int *p_offset;          // 存储顶点在p_edges当中的起始位置
    std::vector<unsigned int> edges; // 存储边

    /*图的一些属性*/
    double avgDeg; // 平均度

    typedef bool (*p_GraphProcessFunc)(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                                       unsigned int **p_degrees, unsigned int **p_offset, std::vector<unsigned int> &edges); // 一个处理图的函数指针类型
    /* 处理图可能要用到的变量 */
    static p_GraphProcessFunc p_processPrimitiveGraphLists[PROCESS_GRAPH_FUNC_ARRAY_SIZE]; // 图处理函数列表
    static bool listInited;
    p_GraphProcessFunc p_processGraph; // 图处理函数指针

public:
    /* @param fileDir:要读取的文件位置*/
    Graph(std::string fileDir);
    /* @return vertex:图的度数*/
    unsigned int GetVertex(void) const { return vertex; };
    /* @return vertex:图的边数*/
    unsigned int GetEdge(void) const { return edge; };
    /* @return vertex:图的平均度*/
    unsigned int GetAvgDeg(void) const { return avgDeg; };
    ~Graph();

private:
    /*  @param  func:要插入的处理函数
        @return true:成功插入
        @return false:因为列表已满，插入失败。*/
    bool InsertGraphProcess(p_GraphProcessFunc func);
    /*  @return true:初始化成功
        @return false:初始化失败*/
    bool InitGraphProcessList();
    /*  @return true: 成功读取处理后的图
        @return false: 读取文件失败，该图还没有被处理*/
    bool ReadProcessedGraph();
    /* @return func:处理该文件所需要的图处理函数*/
    p_GraphProcessFunc GetGraphProcessFunc();
};

bool Graph::listInited = false;