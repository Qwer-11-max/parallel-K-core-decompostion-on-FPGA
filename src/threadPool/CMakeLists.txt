find_package(Threads REQUIRED)
add_library(ThreadPool STATIC threadPool.h threadPool.cpp)

target_link_libraries(ThreadPool PUBLIC
    "${CMAKE_THREAD_LIBS_INIT}"
    Threads::Threads
)

install(FILES threadPool.h DESTINATION ${PROJECT_SOURCE_DIR}/include)
install(TARGETS ThreadPool DESTINATION ${PROJECT_SOURCE_DIR}/lib)