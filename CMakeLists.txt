cmake_minimum_required(VERSION 3.28)

project(KCoreFPGA LANGUAGES CXX VERSION 0.0.1)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src/graphProcessFunc)
configure_file(configure.h.in configure.h)
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src/include"
)