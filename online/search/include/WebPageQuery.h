#ifndef __WEBPAGEQUERY_H__
#define __WEBPAGEQUERY_H__

/* 加载 pagelib.dat, offsetlib.dat, indexlib.dat，完成查询打分 */

#include "Configuration.h"
#include "WebPage.h"
#include <vector>
#include <unordered_map>
#include <utility>
#include <string>

using std::vector;
using std::unordered_map;
using std::pair;
using std::string;

class WebPageQuery
{
public:
    static WebPageQuery &getInstance();
    vector<WebPage> doQuery(const vector<string> &tokens);

private:
    WebPageQuery();
    ~WebPageQuery() = default;
    WebPageQuery(const WebPageQuery &) = delete;
    WebPageQuery &operator=(const WebPageQuery &) = delete;

private:
    void loadOffsetLib();
    void loadIndexLib();
    int getTopk();
    WebPage loadPage(int docid);
    string findDoc(int docid);

private:
    Configuration &_conf;
    const string &_pageLib;
    vector<WebPage> _pages;
    unordered_map<int, pair<int, int>> _offsetLib;
    unordered_map<string, vector<pair<int, double>>> _invertIndexLib;
};

#endif
