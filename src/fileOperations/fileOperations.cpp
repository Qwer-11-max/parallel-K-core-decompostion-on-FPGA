#include "fileOperations.h"

#define THREAD_SIZE 3
namespace threadFuncArgs
{
    struct ReadIntArgs
    {
        ifstream &fileIn;
        unsigned int *array;
    };

    struct ReadVectorArgs
    {
        ifstream &fileIn;
        vector<unsigned int> &edges;
    };

    struct WriteIntArgs
    {
        ofstream &fileOut;
        unsigned int *array;
        unsigned int size;
    };

    struct WriteVectorArgs
    {
        ofstream &fileOut;
        vector<unsigned int> &edges;
        unsigned int size;
    };
} // namespace threadFuncArgs

void *ReadDegrees(void *DegreesInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::ReadIntArgs *)DegreesInfo;
    auto &file = temp->fileIn;
    auto array = temp->array;
    // 开始读取

    for (int i = 0; file; i++)
    {
        file >> array[i];
    }
    // 读取完毕

    cout << "度读取完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

void *ReadEdges(void *EdgesInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::ReadVectorArgs *)EdgesInfo;
    auto &file = temp->fileIn;
    auto &array = temp->edges;
    // 开始读取

    for (int i = 0; file; i++)
    {
        file >> array[i];
    }
    // 读取完毕

    cout << "边读取完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

void *ReadOffset(void *OffsetInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::ReadIntArgs *)OffsetInfo;
    auto &file = temp->fileIn;
    auto array = temp->array;
    // 开始读取

    for (int i = 0; file; i++)
    {
        file >> array[i];
    }
    // 读取完毕

    cout << "偏移读取完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

bool ParallelReadStdGraph(string &fileDir, unsigned int &vertex, unsigned int &edge, unsigned int **p_degrees, unsigned int **p_offset, vector<unsigned int> &edges)
{
    //======检查文件是否存在，不存在返回错误信息=======
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
    std::ifstream fileDegrees, fileOffset, fileEdge;
    fileDegrees.open(DEGREES_FILE(fileName));
    fileOffset.open(OFFSET_FILE(fileName));
    fileEdge.open(EDGES_FILE(fileName));
    if (!fileDegrees && !fileOffset && !fileEdge)
    {
        std::cout << fileName << "未被处理过" << std::endl;
        return false;
    }
    //======打包参数并调用子线程执行函数========
    std::cout << fileName + "已被处理过，开始读取" << std::endl;
    fileDegrees >> vertex;
    fileEdge >> edge;
    *p_degrees = new unsigned int[vertex];
    *p_offset = new unsigned int[vertex];
    edges.resize(edge);

    // 调用子线程
    pthread_t threadId[THREAD_SIZE];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    threadFuncArgs::ReadIntArgs t1 = {fileDegrees, *p_degrees};
    int rc = pthread_create(&threadId[0], &attr, ReadDegrees, &t1);
    threadFuncArgs::ReadIntArgs t2 = {fileOffset, *p_offset};
    int rc1 = pthread_create(&threadId[1], &attr, ReadOffset, &t2);
    threadFuncArgs::ReadVectorArgs t3 = {fileEdge, edges};
    int rc2 = pthread_create(&threadId[2], &attr, ReadEdges, &t3);

    if (rc && rc1 && rc2)
    {
        cout << "pthread_create创建线程失败" << endl;
        exit(-1);
    }
    pthread_attr_destroy(&attr);

    // 等待完成读取，打印消息
    for (int i = 0; i < THREAD_SIZE; i++)
    {
        int rc = pthread_join(threadId[i], NULL);
        if (rc)
        {
            cout << "pthread_join() 执行失败" << endl;
            exit(-1);
        }
    }

    cout << "标准图读取完毕!!!" << endl;
    fileDegrees.close();
    fileOffset.close();
    fileEdge.close();
    return true;
}

void *WriteDegrees(void *DegreesInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::WriteIntArgs *)DegreesInfo;
    auto &file = temp->fileOut;
    auto array = temp->array;
    unsigned int size = temp->size;
    // 开始写入

    for (int i = 0; i < size; i++)
    {
        file << array[i] << endl;
        ;
    }
    // 写入完毕

    cout << "度写入完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

void *WriteEdges(void *EdgesInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::WriteVectorArgs *)EdgesInfo;
    auto &file = temp->fileOut;
    auto &array = temp->edges;
    unsigned int size = temp->size;
    // 开始写入

    for (int i = 0; i < size; i++)
    {
        file << array[i] << endl;
        ;
    }
    // 写入完毕

    cout << "边写入完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

void *WriteOffset(void *OffsetInfo)
{
    // 解包参数
    auto *temp = (threadFuncArgs::WriteIntArgs *)OffsetInfo;
    auto &file = temp->fileOut;
    auto array = temp->array;
    unsigned int size = temp->size;
    // 开始写入

    for (int i = 0; i < size; i++)
    {
        file << array[i] << endl;
    }
    // 写入完毕

    cout << "偏移写入完毕" << endl;
    pthread_exit(NULL);
    return nullptr;
}

void ParallelWriteStdGraph(string &fileDir, unsigned int vertex, unsigned int edge, unsigned int *p_degrees, unsigned int *p_offset, vector<unsigned int> &edges)
{
    //=======打开文件并检查======
    std::string fileName = fileDir.substr(fileDir.find_last_of('/'), fileDir.find_last_of('.') - fileDir.find_last_of('/'));
    std::ofstream fileDegrees, fileOffset, fileEdge;
    fileDegrees.open(DEGREES_FILE(fileName));
    fileOffset.open(OFFSET_FILE(fileName));
    fileEdge.open(EDGES_FILE(fileName));

    if (!fileEdge && !fileDegrees && !fileOffset)
    {
        std::cout << "输出文件错误" << std::endl;
        exit(-1);
    }
    //=======写入文件=======
    fileDegrees << vertex;
    fileEdge << edge;
    // 调用子线程
    pthread_t threadId[THREAD_SIZE];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    threadFuncArgs::WriteIntArgs t1 = {fileDegrees, p_degrees, vertex};
    int rc = pthread_create(&threadId[0], &attr, WriteDegrees, &t1);
    threadFuncArgs::WriteIntArgs t2 = {fileOffset, p_offset, vertex};
    int rc1 = pthread_create(&threadId[1], &attr, WriteOffset, &t2);
    threadFuncArgs::WriteVectorArgs t3 = {fileEdge, edges, edge};
    int rc2 = pthread_create(&threadId[2], &attr, WriteEdges, &t3);

    if (rc && rc1 && rc2)
    {
        cout << "pthread_create创建线程失败" << endl;
        exit(-1);
    }
    pthread_attr_destroy(&attr);

    // 等待完成读取，打印消息
    for (int i = 0; i < THREAD_SIZE; i++)
    {
        int rc = pthread_join(threadId[i], NULL);
        if (rc)
        {
            cout << "pthread_join() 执行失败" << endl;
            exit(-1);
        }
    }
    //=======写入完毕=======
    std::cout << "以下文件写入完毕:" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Degrees.txt" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Offset.txt" << std::endl;
    std::cout << PROCESSED_GRAPH_DIR + fileName + "_Edges.txt" << std::endl;
    // 关闭文件
    fileDegrees.close();
    fileOffset.close();
    fileEdge.close();
}

#undef THREAD_SIZE