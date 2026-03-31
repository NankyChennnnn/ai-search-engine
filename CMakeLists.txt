cmake_minimum_required(VERSION 3.10)
project(ai-search-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_OFFLINE_WORD "Build offline word module" ON)
option(BUILD_OFFLINE_PAGE "Build offline page module" OFF)
option(BUILD_ONLINE "Build online module" OFF)

if(BUILD_OFFLINE_WORD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/offline/word/CMakeLists.txt")
    add_subdirectory(offline/word)
endif()

if(BUILD_OFFLINE_PAGE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/offline/page/CMakeLists.txt")
    add_subdirectory(offline/page)
endif()

if(BUILD_ONLINE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/online/CMakeLists.txt")
    add_subdirectory(online)
endif()

