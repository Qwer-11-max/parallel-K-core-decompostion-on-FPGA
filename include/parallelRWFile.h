#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <pthread.h>


using namespace std;

/*  @param fileName:要读取的文件名
    @param vertex:存储顶点数
    @param edge:存储边数
    @param p_degree:存储度的数组的指针
    @param p_offset:存储偏移的数组的指针
    @param edges:存储边的数组*/
bool ParallelReadStdGraph(string &fileDir, unsigned int &vertex, unsigned int &edge,
                          unsigned int **p_degrees, unsigned int **p_offset, vector<unsigned int> &edges);
/*  @param fileName:要写入的文件名
    @param vertex:顶点数
    @param edge:边数
    @param p_degree:存储边的数组
    @param p_offset:存储偏移值的数组
    @param edges:存储边的数组*/
void ParallelWriteStdGraph(string &fileDir, unsigned int vertex, unsigned int edge,
                           unsigned int *p_degrees, unsigned int *p_offset, vector<unsigned int> &edges);