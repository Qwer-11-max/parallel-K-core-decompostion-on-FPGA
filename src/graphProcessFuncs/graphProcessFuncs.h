#pragma once

/*用于声明图处理函数的头文件*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "parallelRWFile.h"

/*  @param fileDir:要读取的文件路径
    @param vertex:顶点数
    @param edge:边数
    @param degrees:存储边的数组
    @param offsets:存储偏移值的数组
    @param adjs:存储边的数组*/

/// @brief 处理NVE文件，该文件格式如下
/// # 顶点数量
/// src dst
/// src dst
/// @param fileDir 文件的绝对路径，用于打开文件
/// @param vertex 顶点数
/// @param edge 边数
/// @param degrees 存储顶点的数组
/// @param offsets 存储偏移值的数组
/// @param adjs 存储邻接点
/// @return 成功处理返回true，否则返回false
bool GraphProcessFunc_NVE(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                          std::vector<unsigned int> &degrees, std::vector<unsigned int> &offset, std::vector<unsigned int> &adjs);