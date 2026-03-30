cmake_minimum_required(VERSION 3.10)
project(ai-search-engine)

# 告诉 CMake 导出编译命令信息给编辑器看
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 src 下的所有源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 生成可执行文件到 bin 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(engine ${SOURCES})

# 核心步骤：添加头文件搜索路径
target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/include)
message(STATUS "====== 当前项目的真实根目录是: ${PROJECT_SOURCE_DIR} ======")
