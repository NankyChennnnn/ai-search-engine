#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>
#include "cppjieba/Jieba.hpp"
#include "simhash/Simhasher.hpp"

using std::vector;
using std::string;
using std::unordered_map;
using std::set;
using std::pair;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class PageLibPreprocessor
{
public:
    PageLibPreprocessor();
    ~PageLibPreprocessor();

    void cutRedundantPage(vector<RssItem> &rss, 
                          const simhash::Simhasher &simhasher); // 网页去重
    void buildInvertIndexMap(vector<RssItem> &rss, // 建立倒排索引库
                             unordered_map<string, set<pair<int , double>>> &invertIndex,
                             const cppjieba::Jieba &jieba);
    void storeOnDisk(); // 存两个dat文件

private:
    vector<string> _pageList;
    unordered_map<string, set<pair<int , double>>> _invertIndexLib; // <word, <docid, weight>>
};

#endif
