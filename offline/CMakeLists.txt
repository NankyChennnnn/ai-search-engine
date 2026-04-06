cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ai-search-engine-offline LANGUAGES CXX)
endif()

add_subdirectory(word)
add_subdirectory(page)

