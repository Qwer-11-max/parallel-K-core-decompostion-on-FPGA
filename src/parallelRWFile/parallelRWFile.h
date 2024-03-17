#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

#define FILE_MAX_SIZE 0x4000000 //单个文件数据的最大条数

using namespace std;

/// @brief 并行的读取标准文件
/// @param fileDir 文件的绝对路径，用于提取文件名
/// @param vertex 顶点数
/// @param edge 边数
/// @param degrees 存储顶点的数组
/// @param offsets 存储偏移值的数组
/// @param adjs 存储邻接点
/// @return 成功读取返回Ture，否则返回false
bool ParallelReadStdGraph(string &fileDir, unsigned int &vertex, unsigned int &edge,
                          std::vector<unsigned int> &degrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs);

/// @brief 将标准图并行的写入文件中，文件分为四个大类，第一种是存储图基本信息，其余几种是用来存储图顶点，偏移以及邻接点
/// @param fileDir 文件的绝对路径，用于提取文件名
/// @param vertex 顶点数
/// @param edge 边数
/// @param degrees 存储顶点的数组
/// @param offsets 存储偏移值的数组
/// @param adjs 存储邻接点
void ParallelWriteStdGraph(string &fileDir, unsigned int vertex, unsigned int edge,
                           std::vector<unsigned int> &degrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs);