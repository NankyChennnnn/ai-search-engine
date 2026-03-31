cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ai-search-engine-offline-word LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(THIRD_PARTY_DIR "${REPO_ROOT}/third_party")
set(CPPJIEBA_DIR "${THIRD_PARTY_DIR}/cppjieba")
set(CPPJIEBA_DICT_DIR "${CPPJIEBA_DIR}/dict")

if(NOT EXISTS "${CPPJIEBA_DIR}/Jieba.hpp")
    message(FATAL_ERROR "cppjieba not found: ${CPPJIEBA_DIR}")
endif()

file(GLOB WORD_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

add_executable(word_producer ${WORD_SOURCES})

set_target_properties(word_producer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

target_include_directories(word_producer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${THIRD_PARTY_DIR}"
)

target_compile_definitions(word_producer PRIVATE
    DEFAULT_CONF_PATH="${CMAKE_CURRENT_SOURCE_DIR}/conf/word.conf"
    CPPJIEBA_DICT_PATH="${CPPJIEBA_DICT_DIR}/jieba.dict.utf8"
    CPPJIEBA_HMM_PATH="${CPPJIEBA_DICT_DIR}/hmm_model.utf8"
    CPPJIEBA_USER_DICT_PATH="${CPPJIEBA_DICT_DIR}/user.dict.utf8"
    CPPJIEBA_IDF_PATH="${CPPJIEBA_DICT_DIR}/idf.utf8"
    CPPJIEBA_STOP_WORD_PATH="${CPPJIEBA_DICT_DIR}/stop_words.utf8"
)

message(STATUS "word source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "repo root: ${REPO_ROOT}")
message(STATUS "cppjieba dir: ${CPPJIEBA_DIR}")

