#pragma once

#include <string>
#include <vector>
#include <map>

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
    static p_GraphProcessFunc p_graphProcessFuncList[PROCESS_GRAPH_FUNC_ARRAY_SIZE]; // 图处理函数列表
    static std::map<std::string, int> hash;                                          // 类型和在图处理函数中位置的映射表
    static bool listInited;                                                          // 图像是否已被处理
    bool processFailed;                                                              // 图像处理是否失败

public:
    /*  @param fileDir:要读取的文件位置
        @param initGraph:是否强制处理图 */
    Graph(std::string fileDir,bool initGraph = false);
    /*  @return vertex:图的度数*/
    unsigned int GetVertex() const { return vertex; };
    /*  @return vertex:图的边数*/
    unsigned int GetEdge() const { return edge; };
    /*  @return vertex:图的平均度*/
    unsigned int GetAvgDeg() const { return avgDeg; };
    /*  @return processFailed:图像处理是否失败*/
    bool GetProcessResult() const { return processFailed; };
    ~Graph();

private:
    /*  @param  func:要插入的处理函数
        @param  typeName:函数处理的类型
        @param  loc:函数在列表中的位置
        @return true:成功插入
        @return false:因为列表已满，插入失败。*/
    inline bool InsertGraphProcess(p_GraphProcessFunc func, std::string &&typeName, int loc)
    {
        if (loc >= PROCESS_GRAPH_FUNC_ARRAY_SIZE)
        {
            std::cout << "函数过多，请增加列表容量" << std::endl;
            return false;
        }
        p_graphProcessFuncList[loc] = func;
        hash[typeName] = loc;
        return true;
    };
    /*  @return true:初始化成功
        @return false:初始化失败*/
    bool InitGraphProcessList();
    /*  @param  fileDir:要处理的图
        @return true: 成功读取处理后的图
        @return false: 读取文件失败，该图还没有被处理*/
    bool ReadProcessedGraph(std::string fileDir);
    /*  @param  fileDir:要处理的图
        @return func:处理该文件所需要的图处理函数*/
    inline p_GraphProcessFunc GetGraphProcessFunc(std::string fileDir)
    {
        std::string fileType = fileDir.substr(fileDir.find_last_of('.') + 1);
        if (hash.count(fileType))
            return p_graphProcessFuncList[hash[fileType]];
        else
        {
            std::cout << "没有处理" + fileType + "的函数，请更换文件或编写处理该文件的函数" << std::endl;
            return nullptr;
        }
    }
};