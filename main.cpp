#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

#include "configure.h"
#include "graph.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "输入参数过少，请输入要处理的文件" << endl;
        return 0;
    }
    std::string fileDir(argv[1]);
    
    Graph grh(fileDir,false);
    grh.PKC();
    return 0;
}