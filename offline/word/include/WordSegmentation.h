#ifndef __WORDSEGMENTATION_H__
#define __WORDSEGMENTATION_H__

#include "cppjieba/Jieba.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// 最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char * const DICT_PATH = CPPJIEBA_DICT_PATH;

// 隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char * const HMM_PATH = CPPJIEBA_HMM_PATH;

// 用户自定义词典路径
const char * const USER_DICT_PATH = CPPJIEBA_USER_DICT_PATH;

// IDF路径
const char* const IDF_PATH = CPPJIEBA_IDF_PATH;

// 停用词路径
const char* const STOP_WORD_PATH = CPPJIEBA_STOP_WORD_PATH;

class WordSegmentation
{
public:
    WordSegmentation();
    vector<string> operator()(const string str);

private:
    cppjieba::Jieba _jieba;
};

#endif
