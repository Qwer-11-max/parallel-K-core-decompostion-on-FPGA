add_library(Graph STATIC graph.cpp)

target_link_libraries(Graph
    GraphProcessFuncs
    FileOperations
)
target_include_directories(Graph PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/include"
)

install(TARGETS Graph LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib)