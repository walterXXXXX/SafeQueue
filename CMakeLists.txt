cmake_minimum_required(VERSION 3.22.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project(Safe_Queue)

add_executable(safe_queue main.cpp SafeQueue.h ThreadPool.h ThreadPool.cpp)