cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ai-search-engine-offline-page LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(THIRD_PARTY_DIR "${REPO_ROOT}/third_party")
set(CPPJIEBA_DIR "${THIRD_PARTY_DIR}/cppjieba")
set(SIMHASH_DIR "${THIRD_PARTY_DIR}/simhash")

if(NOT EXISTS "${CPPJIEBA_DIR}/Jieba.hpp")
    message(FATAL_ERROR "cppjieba not found: ${CPPJIEBA_DIR}")
endif()

if(NOT EXISTS "${SIMHASH_DIR}/Simhasher.hpp")
    message(FATAL_ERROR "simhash not found: ${SIMHASH_DIR}")
endif()

file(GLOB PAGE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

add_executable(page_processor 
    ${PAGE_SOURCES}
    ${THIRD_PARTY_DIR}/tinyxml2/tinyxml2.cc)

set_target_properties(page_processor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

target_include_directories(page_processor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${THIRD_PARTY_DIR}"
)

target_compile_definitions(page_processor PRIVATE
    PAGE_CONF_PATH="${CMAKE_CURRENT_SOURCE_DIR}/conf/page.conf"
    JIEBA_CONF_PATH="${CMAKE_CURRENT_SOURCE_DIR}/conf/jieba.conf"
)

message(STATUS "page source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "repo root: ${REPO_ROOT}")
message(STATUS "cppjieba dir: ${CPPJIEBA_DIR}")
message(STATUS "simhash dir: ${SIMHASH_DIR}")
