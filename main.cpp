#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <chrono>
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

    cout << "开始处理文件：" << argv[1] << endl;
    auto start = chrono::steady_clock::now();
    Graph grh(fileDir, false);
    auto mid = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(mid - start);
    cout << "预处理用时：" << duration.count() << " ms" << endl;
    grh.PKC();
    auto end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - mid);
    cout << "PKC算法用时:" << duration.count() << " ms" << endl;
    return 0;
}