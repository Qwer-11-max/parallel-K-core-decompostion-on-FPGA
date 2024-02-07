# 配置图处理函数库
file(GLOB GRAPH_PROCESS_FUNC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/graphProcessFunc_*.cpp)
add_library(GraphProcessFuncs STATIC ${GRAPH_PROCESS_FUNC_SRC})
target_include_directories(GraphProcessFuncs PUBLIC
    "${PROJECT_BINARY_DIR}"
)

install(TARGETS GraphProcessFuncs DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES ${PROJECT_SOURCE_DIR}/src/include/graphProcessFuncs.h DESTINATION ${PROJECT_SOURCE_DIR}/include)