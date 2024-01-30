# 项目信息
cmake_minimum_required(VERSION 3.28)
project(KCoreFPGA VERSION 0.0.1 LANGUAGES C CXX)

# 测试信息
include(CTest)
enable_testing()

# 默认配置文件
include_directories(${PROJECT_SOURCE_DIR}/src/include)
configure_file(configure.h.in configure.h)
add_subdirectory(src/graphProcessFuncs)
# KCoreFPGA项目配置
add_executable(KCoreFPGA main.cpp)
target_include_directories(KCoreFPGA PUBLIC
    "${PROJECT_BINARY_DIR}"
)
target_link_libraries(KCoreFPGA PUBLIC GraphProcessFuncs)

# 打包信息
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
