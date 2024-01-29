file(GLOB GRAPH_PROCESS_FUNC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/graphProcessFunc_*.cpp)

add_library(ProcessGraphFuncs STATIC ${GRAPH_PROCESS_FUNC_SRC})