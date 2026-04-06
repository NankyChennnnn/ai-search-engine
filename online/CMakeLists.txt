cmake_minimum_required(VERSION 3.10)

project(ai-search-engine-online LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(THIRD_PARTY_DIR "${REPO_ROOT}/third_party")

set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common/")
set(RECOMMEND_DIR "${CMAKE_CURRENT_SOURCE_DIR}/recommend/")
set(SEARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/search/")
set(CACHE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cache/")
set(SERVER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/server/")

file(GLOB ONLINE_SOURCES CONFIGURE_DEPENDS
    "${COMMON_DIR}/src/*.cc"
    "${RECOMMEND_DIR}/src/*.cc"
    "${SEARCH_DIR}/src/*.cc"
    "${CACHE_DIR}/src/*.cc"
    "${SERVER_DIR}/*.cc")

add_executable(server ${ONLINE_SOURCES})

set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

target_include_directories(server PRIVATE
    "${COMMON_DIR}/include"
    "${RECOMMEND_DIR}/include"
    "${SEARCH_DIR}/include"
    "${CACHE_DIR}/include"
    "${THIRD_PARTY_DIR}"
)

message(STATUS "page source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "repo root: ${REPO_ROOT}")
