cmake_minimum_required(VERSION 3.10)
project(SNL-Compiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# 收集源文件
file(GLOB_RECURSE SOURCES src/*.cpp)

# 创建可执行文件
add_executable(SNLc ${SOURCES})

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
