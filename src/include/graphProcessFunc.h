#pragma once

/*用于声明图处理函数的头文件*/

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "configure.h" //一些用得到的宏

/*NVE文件：后缀为nve，格式如下：
 *#vertex number
 *src dst   //其中每个顶点的的边是聚集在一起的
 *src dst
 *.....
 */
bool GraphProcessFunc_NVE(std::string fileDir, unsigned int &vertex, unsigned int &edge,
                          unsigned int **p_degrees, unsigned int **p_offset, std::vector<unsigned int> &edges);