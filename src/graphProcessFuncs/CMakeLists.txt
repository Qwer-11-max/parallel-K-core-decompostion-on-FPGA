# 打包当前文件夹的所有图处理函数文件
file(GLOB GRAPH_PROCESS_FUNC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/graphProcessFunc_*.cpp;graphProcessFuncs.h)
# 创建图处理函数库
add_library(GraphProcessFuncs STATIC ${GRAPH_PROCESS_FUNC_SRC})
target_include_directories(GraphProcessFuncs PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/parallelRWFile"
)

# 安装图处理函数库
install(TARGETS GraphProcessFuncs DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES graphProcessFuncs.h DESTINATION ${PROJECT_SOURCE_DIR}/include)