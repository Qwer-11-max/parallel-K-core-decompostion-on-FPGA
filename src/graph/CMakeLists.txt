add_library(Graph STATIC graph.h graph.cpp PKC.cpp)

target_link_libraries(Graph
    PUBLIC GraphProcessFuncs
    PUBLIC ParallelRWFile
    PUBLIC ThreadPool
)
target_include_directories(Graph PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/graphProcessFuncs"
    "${PROJECT_SOURCE_DIR}/src/parallelRWFile"
    "${PROJECT_SOURCE_DIR}/src/threadPool"
)

install(TARGETS Graph DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES graph.h DESTINATION ${PROJECT_SOURCE_DIR}/include)