#include "WordSegmentation.h"

WordSegmentation::WordSegmentation()
: _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
{
    cout << "jieba init" << endl;
}

vector<string> WordSegmentation::operator()(const string str)
{
    // 返回分词结果
    vector<string> words;
    _jieba.CutAll(str, words);
    return words;
}

