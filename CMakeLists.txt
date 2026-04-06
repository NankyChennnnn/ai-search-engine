cmake_minimum_required(VERSION 3.10)
project(ai-search-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_OFFLINE_WORD "Build offline word module" ON)
option(BUILD_OFFLINE_PAGE "Build offline page module" ON)
option(BUILD_ONLINE "Build online module" ON)

add_subdirectory(offline/word)
add_subdirectory(offline/page)

add_subdirectory(online)

