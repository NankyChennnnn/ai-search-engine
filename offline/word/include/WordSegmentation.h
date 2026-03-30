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
const char * const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";

// 隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char * const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";

// 用户自定义词典路径
const char * const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";

// IDF路径
const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";

// 停用词路径
const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

class WordSegmentation
{
public:
    WordSegmentation();
    vector<string> operator()(const string str);

private:
    cppjieba::Jieba _jieba;
};

#endif
