# gbk
cmake_minimum_required(VERSION 3.29)

# 生成器设置
if(WIN32)
    set(CMAKE_GENERATOR "MinGW Makefiles")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_GENERATOR "Unix Makefiles")
else()
    message("Warning: not Windows or Linux")
endif()

# 项目
project(RouteOptimise 
        LANGUAGES CXX)

# C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# 编译选项，开启所有警告，将警告视为错误
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++20")

# 设置源文件路径
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")
# 设置头文件包含路径
include_directories(${CMAKE_SOURCE_DIR}/include)
# 设置可执行程序生成路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

add_executable(ros ${SOURCE_FILES})