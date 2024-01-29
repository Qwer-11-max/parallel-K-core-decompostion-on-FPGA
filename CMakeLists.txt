cmake_minimum_required(VERSION 3.28)
project(KCoreFPGA VERSION 0.0.1 LANGUAGES C CXX)

include(CTest)
enable_testing()

configure_file(configure.h.in configure.h)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
add_executable(KCoreFPGA main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
