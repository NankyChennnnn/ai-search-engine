#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

// 完成cppjieba介入和停用词的处理
#include "Configuration.h"
#include "cppjieba/Jieba.hpp"
#include <string>
#include <vector>
#include <unordered_set>

using std::string;
using std::vector;
using std::unordered_set;

class SplitTool
{
public:
    static SplitTool &getInstance();
    // For WebSearch (need override if others need this func)
    vector<string> cutAndFilter(const string &text);
    // For ZH words recommender
    vector<string> decodeRunesInString(const string &query);

private:
    void buildStopWordsMap();
    bool isValidWord(const string &word);
    bool isStopWord(const string &word);

private:
    SplitTool();
    ~SplitTool() = default;
    SplitTool(const SplitTool &) = delete;
    SplitTool &operator=(const SplitTool &) = delete;

private:
    Configuration &_conf;
    cppjieba::Jieba _jieba;
    unordered_set<string> _stopWords;
};

#endif
