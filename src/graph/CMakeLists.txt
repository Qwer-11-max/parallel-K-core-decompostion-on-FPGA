add_library(Graph STATIC graph.cpp)

target_link_libraries(Graph
    PUBLIC GraphProcessFuncs
    PUBLIC ParallelRWFile
)
target_include_directories(Graph PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/include"
)

install(TARGETS Graph DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES ${PROJECT_SOURCE_DIR}/src/include/graph.h DESTINATION ${PROJECT_SOURCE_DIR}/include)