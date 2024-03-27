#include "parallelRWFile.h"
#include "configure.h"
#include "threadPool.h"

#define MODE_VERTEX 0
#define MODE_OFFSET 1
#define MODE_ADJS 2

int ReadGraph(string &fileName, int id, unsigned int hardwareCount, vector<unsigned int> &array, int mode)
{
    // 打开要读取的文件
    ifstream targetFile;
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
    targetFile.open(targetFileName);
    if (!targetFile)
        cout << "文件打开失败，未能读取数据" << endl;
    // 对文件进行读取
    int count = 0;
    unsigned int n = array.size(); // 要读取的数组大小
    for (unsigned int i = 0; i < n; i += hardwareCount)
    {
        unsigned int loc = i + id;
        if (loc < n)
        {
            targetFile >> array[loc];
            count++;
        }
    }
    targetFile.close();
    // 读取完毕，返回工作线程ID
    return count;
}

int WriteGraph(string &fileName, int id, unsigned int hardwareCount, vector<unsigned int> &array, int mode)
{
    // 打开要写入的文件
    ofstream targetFile;
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
    targetFile.open(targetFileName);
    if (!targetFile)
        cout << "文件打开失败，未能读取数据" << endl;
    // 对文件进行写入
    int count = 0;
    unsigned int n = array.size(); // 要写入的数组大小
    for (int i = 0; i < n; i += hardwareCount)
    {
        unsigned int loc = i + id;
        if (loc < n)
        {
            targetFile << array[loc] << endl;
            count++;
        }
    }
    targetFile.close();
    // 写入完毕，返回工作线程ID
    return count;
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

    vector<future<int>> results;
    for (int i = 0; i < hardware_concurrency; i++)
    {
        pool.submit(ReadGraph, fileName, i, hardware_concurrency, std::ref(degrees), MODE_VERTEX);
        pool.submit(ReadGraph, fileName, i, hardware_concurrency, std::ref(offsets), MODE_OFFSET);
        results.emplace_back(pool.submit(ReadGraph, fileName, i, hardware_concurrency, std::ref(adjs), MODE_ADJS));
    }

    for (auto &result : results)
        result.get();

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

    vector<future<int>> results;
    for (int i = 0; i < hardware_concurrency; i++)
    {
        pool.submit(WriteGraph, fileName, i, hardware_concurrency, std::ref(degrees), MODE_VERTEX);
        pool.submit(WriteGraph, fileName, i, hardware_concurrency, std::ref(offsets), MODE_OFFSET);
    }

    for (int i = 0; i < hardware_concurrency; i++)
    {
        results.emplace_back(pool.submit(WriteGraph, fileName, i, hardware_concurrency, std::ref(adjs), MODE_ADJS));
    }
    int count = 0;
    for (auto &result : results)
        count += result.get();

    pool.shutdown();

    cout << "文件写入完成" << endl;
}