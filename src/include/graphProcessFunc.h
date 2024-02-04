#pragma once

/*用于声明图处理函数的头文件*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "configure.h" //一些用得到的宏
#include "fileOperations.h"

/*NVE文件：后缀为nve，格式如下：
 *#vertex number
 *src dst   //其中每个顶点的的边是聚集在一起的
 *src dst
 *.....
 */
/*  @param fileDir:要读取的文件路径
    @param vertex:顶点数
    @param edge:边数
    @param p_degree:存储边的数组
    @param p_offset:存储偏移值的数组
    @param edges:存储边的数组*/
bool GraphProcessFunc_NVE(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                          unsigned int **p_degrees, unsigned int **p_offset, std::vector<unsigned int> &edges);