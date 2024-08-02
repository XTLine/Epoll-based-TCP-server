cmake_minimum_required(VERSION 3.9)

project(reactor_server)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
add_executable(reactor_server main.cpp reactor_server.cpp)