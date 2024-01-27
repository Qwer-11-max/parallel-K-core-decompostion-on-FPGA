#pragma once

#include<string>

#include "configure.h"
//用于存储图信息，包括顶点，边等信息
class Graph
{
private:
    /* data */
    unsigned int    vertex; //顶点数量
    unsigned int    edge;   //边数
    unsigned int    *p_degrees; //存储顶点度数
    unsigned int    *p_offset; //存储顶点在p_edges当中的起始位置
    unsigned int    *p_edges; //存储边

    double          avgDeg;   //平均度
public:
    Graph(std::string fileDir);
    ~Graph();
};