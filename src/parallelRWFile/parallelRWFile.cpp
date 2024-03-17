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

int ReadGraph(string &fileName, int id, int loc, vector<unsigned int> array, int mode)
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

int WriteGraph(string &fileName, int id, int loc, vector<unsigned int> array, int mode)
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

    //=======调用线程池写入文件========
    int hardware_concurrency = thread::hardware_concurrency();
    ThreadPool pool(hardware_concurrency);
    pool.init();

    int iter = ceil(vertex / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(ReadGraph, fileName, i, i * iter, degrees, MODE_VERTEX);
        pool.submit(ReadGraph, fileName, i, i * iter, offsets, MODE_OFFSET);
    }

    iter = ceil(edge / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(ReadGraph, fileName, i, i * iter, adjs, MODE_ADJS);
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
        pool.submit(WriteGraph, fileName, i, i * iter, degrees, MODE_VERTEX);
        pool.submit(WriteGraph, fileName, i, i * iter, offsets, MODE_OFFSET);
    }

    iter = ceil(double(edge) / FILE_MAX_SIZE);
    for (int i = 0; i < iter; i++)
    {
        pool.submit(WriteGraph, fileName, i, i * iter, adjs, MODE_ADJS);
    }

    auto future1 = pool.submit(WaitProcessFinish, 1);

    int res = future1.get();

    pool.shutdown();

    cout << "文件写入完成" << endl;
}

// #define THREAD_SIZE 3
// namespace threadFuncArgs
// {
//     struct ReadIntArgs
//     {
//         ifstream &fileIn;
//         unsigned int *array;
//     };

//     struct ReadVectorArgs
//     {
//         ifstream &fileIn;
//         vector<unsigned int> &adjs;
//     };

//     struct WriteIntArgs
//     {
//         ofstream &fileOut;
//         unsigned int *array;
//         unsigned int size;
//     };

//     struct WriteVectorArgs
//     {
//         ofstream &fileOut;
//         vector<unsigned int> &adjs;
//         unsigned int size;
//     };
// } // namespace threadFuncArgs

// void *ReadDegrees(void *DegreesInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::ReadIntArgs *)DegreesInfo;
//     auto &file = temp->fileIn;
//     auto array = temp->array;
//     // 开始读取

//     for (int i = 0; file; i++)
//     {
//         file >> array[i];
//     }
//     // 读取完毕

//     cout << "度读取完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// void *ReadEdges(void *EdgesInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::ReadVectorArgs *)EdgesInfo;
//     auto &file = temp->fileIn;
//     auto &array = temp->adjs;
//     // 开始读取

//     for (int i = 0; file; i++)
//     {
//         file >> array[i];
//     }
//     // 读取完毕

//     cout << "边读取完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// void *ReadOffset(void *OffsetInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::ReadIntArgs *)OffsetInfo;
//     auto &file = temp->fileIn;
//     auto array = temp->array;
//     // 开始读取

//     for (int i = 0; file; i++)
//     {
//         file >> array[i];
//     }
//     // 读取完毕

//     cout << "偏移读取完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// bool ParallelReadStdGraph(string &fileDir, unsigned int &vertex, unsigned int &edge, unsigned int **p_degrees, unsigned int **p_offset, vector<unsigned int> &adjs)
// {
//     //======检查文件是否存在，不存在返回错误信息=======
//     std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
//     std::ifstream fileDegrees, fileOffset, fileEdge;
//     fileDegrees.open(DEGREES_FILE(fileName));
//     fileOffset.open(OFFSET_FILE(fileName));
//     fileEdge.open(ADJS_FILE(fileName));
//     if (!fileDegrees && !fileOffset && !fileEdge)
//     {
//         std::cout << fileName << "未被处理过" << std::endl;
//         return false;
//     }
//     //======打包参数并调用子线程执行函数========
//     std::cout << fileName + "已被处理过，开始读取" << std::endl;
//     fileDegrees >> vertex;
//     fileEdge >> edge;
//     *p_degrees = new unsigned int[vertex];
//     *p_offset = new unsigned int[vertex];
//     adjs.resize(edge);

//     // 调用子线程
//     pthread_t threadId[THREAD_SIZE];
//     pthread_attr_t attr;

//     pthread_attr_init(&attr);
//     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//     threadFuncArgs::ReadIntArgs t1 = {fileDegrees, *p_degrees};
//     int rc = pthread_create(&threadId[0], &attr, ReadDegrees, &t1);
//     threadFuncArgs::ReadIntArgs t2 = {fileOffset, *p_offset};
//     int rc1 = pthread_create(&threadId[1], &attr, ReadOffset, &t2);
//     threadFuncArgs::ReadVectorArgs t3 = {fileEdge, adjs};
//     int rc2 = pthread_create(&threadId[2], &attr, ReadEdges, &t3);

//     if (rc && rc1 && rc2)
//     {
//         cout << "pthread_create创建线程失败" << endl;
//         exit(-1);
//     }
//     pthread_attr_destroy(&attr);

//     // 等待完成读取，打印消息
//     for (int i = 0; i < THREAD_SIZE; i++)
//     {
//         int rc = pthread_join(threadId[i], NULL);
//         if (rc)
//         {
//             cout << "pthread_join() 执行失败" << endl;
//             exit(-1);
//         }
//     }

//     cout << "标准图读取完毕!!!" << endl;
//     fileDegrees.close();
//     fileOffset.close();
//     fileEdge.close();
//     return true;
// }

// void *WriteDegrees(void *DegreesInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::WriteIntArgs *)DegreesInfo;
//     auto &file = temp->fileOut;
//     auto array = temp->array;
//     unsigned int size = temp->size;
//     // 开始写入

//     for (int i = 0; i < size; i++)
//     {
//         file << array[i] << endl;
//         ;
//     }
//     // 写入完毕

//     cout << "度写入完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// void *WriteEdges(void *EdgesInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::WriteVectorArgs *)EdgesInfo;
//     auto &file = temp->fileOut;
//     auto &array = temp->adjs;
//     unsigned int size = temp->size;
//     // 开始写入

//     for (int i = 0; i < size; i++)
//     {
//         file << array[i] << endl;
//         ;
//     }
//     // 写入完毕

//     cout << "边写入完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// void *WriteOffset(void *OffsetInfo)
// {
//     // 解包参数
//     auto *temp = (threadFuncArgs::WriteIntArgs *)OffsetInfo;
//     auto &file = temp->fileOut;
//     auto array = temp->array;
//     unsigned int size = temp->size;
//     // 开始写入

//     for (int i = 0; i < size; i++)
//     {
//         file << array[i] << endl;
//     }
//     // 写入完毕

//     cout << "偏移写入完毕" << endl;
//     pthread_exit(NULL);
//     return nullptr;
// }

// void ParallelWriteStdGraph(string &fileDir, unsigned int vertex, unsigned int edge, unsigned int *p_degrees, unsigned int *p_offset, vector<unsigned int> &adjs)
// {
//     //=======打开文件并检查======
//     std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
//     std::ofstream fileDegrees, fileOffset, fileEdge;
//     fileDegrees.open(DEGREES_FILE(fileName));
//     fileOffset.open(OFFSET_FILE(fileName));
//     fileEdge.open(ADJS_FILE(fileName));

//     if (!fileEdge && !fileDegrees && !fileOffset)
//     {
//         std::cout << "输出文件错误" << std::endl;
//         exit(-1);
//     }
//     //=======写入文件=======
//     fileDegrees << vertex;
//     fileEdge << edge;
//     // 调用子线程
//     pthread_t threadId[THREAD_SIZE];
//     pthread_attr_t attr;

//     pthread_attr_init(&attr);
//     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//     threadFuncArgs::WriteIntArgs t1 = {fileDegrees, p_degrees, vertex};
//     int rc = pthread_create(&threadId[0], &attr, WriteDegrees, &t1);
//     threadFuncArgs::WriteIntArgs t2 = {fileOffset, p_offset, vertex};
//     int rc1 = pthread_create(&threadId[1], &attr, WriteOffset, &t2);
//     threadFuncArgs::WriteVectorArgs t3 = {fileEdge, adjs, edge};
//     int rc2 = pthread_create(&threadId[2], &attr, WriteEdges, &t3);

//     if (rc && rc1 && rc2)
//     {
//         cout << "pthread_create创建线程失败" << endl;
//         exit(-1);
//     }
//     pthread_attr_destroy(&attr);

//     // 等待完成读取，打印消息
//     for (int i = 0; i < THREAD_SIZE; i++)
//     {
//         int rc = pthread_join(threadId[i], NULL);
//         if (rc)
//         {
//             cout << "pthread_join() 执行失败" << endl;
//             exit(-1);
//         }
//     }
//     //=======写入完毕=======
//     std::cout << "以下文件写入完毕:" << std::endl;
//     std::cout << PROCESSED_GRAPH_DIR + fileName + "_Degrees.txt" << std::endl;
//     std::cout << PROCESSED_GRAPH_DIR + fileName + "_Offset.txt" << std::endl;
//     std::cout << PROCESSED_GRAPH_DIR + fileName + "_Edges.txt" << std::endl;
//     // 关闭文件
//     fileDegrees.close();
//     fileOffset.close();
//     fileEdge.close();
// }

// #undef THREAD_SIZE