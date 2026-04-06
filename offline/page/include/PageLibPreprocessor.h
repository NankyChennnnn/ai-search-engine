#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>
#include "cppjieba/Jieba.hpp"
#include "simhash/Simhasher.hpp"
#include "XmlReader.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::set;
using std::pair;

class PageLibPreprocessor
{
public:
    PageLibPreprocessor();
    ~PageLibPreprocessor();

    void cutRedundantPage(vector<RssItem> &rss, 
                          const simhash::Simhasher &simhasher); // 网页去重
    void buildInvertIndexMap(vector<RssItem> &rss, // 建立倒排索引库
                             const cppjieba::Jieba &jieba);
    void storeOnDisk(const string path); // 存两个dat文件

private:
    unordered_map<string, set<pair<int , double>>> _invertIndexLib; // <word, <docid, weight>>
};

#endif
