#include "parallelRWFile.h"
#include "configure.h"
#include "threadPool.h"

#define MODE_VERTEX 0
#define MODE_OFFSET 1
#define MODE_ADJS 2

/// @brief 作为最后一个线程压入线程池，当该线程执行完毕时，任务队列为空
/// @param mode 输出模式，1为文件写入，0为文件读取
/// @return 不重要
int WaitProcessFinish(int mode)
{
    if (mode)
        cout << "文件写入即将完成" << endl;
    else
        cout << "文件读取即将完成" << endl;
    return 1;
}

int ReadGraph(string &fileName, int id, int loc, vector<unsigned int> &array, int mode)
{
    // 打开要读取的文件
    ifstream targteFile;
    string targetFileName;
    switch (mode)
    {
    case MODE_VERTEX:
        targetFileName = DEGREES_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    case MODE_OFFSET:
        targetFileName = OFFSET_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    case MODE_ADJS:
        targetFileName = ADJS_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    default:
        cout << "需要读取的文件打开失败，程序退出" << endl;
        exit(0);
        break;
    }
    targteFile.open(targetFileName);
    if (!targteFile)
        cout << "文件打开失败，未能读取数据" << endl;
    // 对文件进行读取
    for (int i = 0; loc + i < array.size() && i < FILE_MAX_SIZE; i++)
    {
        targteFile >> array[loc + i];
    }
    cout << "文件 " << targetFileName << " 读取完毕" << endl;
    targteFile.close();
    // 读取完毕，返回工作线程ID
    return id;
}

int WriteGraph(string &fileName, int id, int loc, vector<unsigned int> &array, int mode)
{
    // 打开要写入的文件
    ofstream targteFile;
    string targetFileName;
    switch (mode)
    {
    case MODE_VERTEX:
        targetFileName = DEGREES_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    case MODE_OFFSET:
        targetFileName = OFFSET_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    case MODE_ADJS:
        targetFileName = ADJS_FILE(fileName) + "_" + to_string(id) + ".txt";
        break;
    default:
        cout << "需要写入的文件打开失败，程序退出" << endl;
        exit(0);
        break;
    }
    targteFile.open(targetFileName);
    if (!targteFile)
        cout << "文件打开失败，未能读取数据" << endl;
    // 对文件进行写入
    for (int i = 0; loc + i < array.size() && i < FILE_MAX_SIZE; i++)
    {
        targteFile << array[loc + i] << endl;
    }
    cout << "文件 " << targetFileName << " 写入完毕" << endl;
    targteFile.close();
    // 写入完毕，返回工作线程ID
    return id;
}

bool ParallelReadStdGraph(string &fileDir, unsigned int &vertex, unsigned int &edge,
                          std::vector<unsigned int> &degrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs)
{
    //=======打开要存储的文件========
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));

    // 存储图关键信息，即顶点数和边数
    ifstream fileKeyInfo;
    fileKeyInfo.open(PROCESSED_GRAPH_DIR + fileName + "_KeyInfo.txt");
    if (!fileKeyInfo)
        return false;
    fileKeyInfo >> vertex >> edge;
    fileKeyInfo.close();
    degrees.resize(vertex);
    offsets.resize(vertex);
    adjs.resize(edge);

    //=======调用线程池写入文件========
    int hardware_concurrency = thread::hardware_concurrency();
    ThreadPool pool(hardware_concurrency);
    pool.init();

    int iter = ceil(double(vertex) / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(ReadGraph, fileName, i, i * iter, std::ref(degrees), MODE_VERTEX);
        pool.submit(ReadGraph, fileName, i, i * iter, std::ref(offsets), MODE_OFFSET);
    }

    iter = ceil(double(edge) / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(ReadGraph, fileName, i, i * iter, std::ref(adjs), MODE_ADJS);
    }

    // 往队列的最后放一个任务，作为结束标记
    auto future1 = pool.submit(WaitProcessFinish, 0);

    int res = future1.get();

    pool.shutdown();
    cout << "文件读取完成" << endl;
    return true;
}

void ParallelWriteStdGraph(string &fileDir, unsigned int vertex, unsigned int edge,
                           std::vector<unsigned int> &degrees, std::vector<unsigned int> &offsets, std::vector<unsigned int> &adjs)
{
    //=======打开要存储的文件========
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));

    // 存储图关键信息，即顶点数和边数
    ofstream fileKeyInfo;
    fileKeyInfo.open(PROCESSED_GRAPH_DIR + fileName + "_KeyInfo.txt");
    fileKeyInfo << vertex << " " << edge;
    fileKeyInfo.close();

    //=======调用线程池写入文件========
    int hardware_concurrency = thread::hardware_concurrency();
    ThreadPool pool(hardware_concurrency);
    pool.init();

    int iter = ceil(double(vertex) / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(WriteGraph, fileName, i, i * iter, std::ref(degrees), MODE_VERTEX);
        pool.submit(WriteGraph, fileName, i, i * iter, std::ref(offsets), MODE_OFFSET);
    }

    iter = ceil(double(edge) / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(WriteGraph, fileName, i, i * iter, std::ref(adjs), MODE_ADJS);
    }

    auto future1 = pool.submit(WaitProcessFinish, 1);

    int res = future1.get();

    pool.shutdown();

    cout << "文件写入完成" << endl;
}