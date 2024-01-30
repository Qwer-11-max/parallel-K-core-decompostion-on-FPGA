add_library(Graph STATIC graph.cpp)

target_link_libraries(Graph PUBLIC
    GraphProcessFuncs
)
target_include_directories(Graph PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/include"
)