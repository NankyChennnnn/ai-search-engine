#ifndef __WORDSEGMENTATION_H__
#define __WORDSEGMENTATION_H__

#include "cppjieba/Jieba.hpp"
#include "JiebaConfig.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

extern JiebaConfig _jiebaConf;

class WordSegmentation
{
public:
    WordSegmentation();
    vector<string> operator()(const string str);

private:
    cppjieba::Jieba _jieba;
};

#endif
