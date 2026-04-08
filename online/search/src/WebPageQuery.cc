#include "WebPageQuery.h"
#include "WebPage.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>

// @TODO
// 1. 完成doQuery内容
// 2. 明确pagelib如何读取索引

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

WebPageQuery::WebPageQuery()
: _conf(Configuration::getInstance())
, _pageLib(_conf.getConfig("pagelib"))
{
    loadOffsetLib();
    loadIndexLib();
}

WebPageQuery &WebPageQuery::getInstance()
{
    static WebPageQuery instance;
    return instance;
}

vector<WebPage> WebPageQuery::doQuery(const vector<string> &tokens)
{
    cout << "[WARNING] Querying \"";
    // 表示docid对应文章的权值总和
    // 如 'h' 和 'o' 都提到了docid = 4的文章
    // 则权值汇总相加，最后比较权值，最大的候选
    unordered_map<int, double> docScores;
    
    // 上层完成拆词（中文/英文/中英混合）
    for (const auto &token : tokens)
    {
        // 找到单元词或字母对应倒排索引
        auto index = _invertIndexLib.find(token);
        if (index == _invertIndexLib.end()) continue;

        cout << index->first;
        
        // 权值加和
        for (const auto &it : index->second)
        {
            int docid = it.first;
            double weight = it.second;
            docScores[docid] += weight;
        }

    }
    cout << "\"..." << endl;
    
    // 按照加和权值排序
    vector<pair<int, double>> rankedDocs(docScores.begin(), docScores.end());
    std::sort(rankedDocs.begin(), rankedDocs.end(), 
              [](const auto &lhs, const auto &rhs) {
                  return lhs.second > rhs.second;
              });
    
    // 获取前topk个候选文章
    vector<WebPage> pages;
    int topk = std::min({getTopk(), static_cast<int>(rankedDocs.size())});
    for (int i = 0; i < topk; ++i)
    {
        int docid = rankedDocs[i].first;
        double weight = rankedDocs[i].second;
        
        // 找出对应文章并插入容器
        pages.push_back(loadPage(docid));
    }

    cout << "[INFO] Query done." << endl;

    return pages;
}

void WebPageQuery::loadOffsetLib()
{
    string path = _conf.getConfig("offsetlib");
    string name = "offsetlib.dat";
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open \"" << name << ".dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Open \"" << name << ".dat\" successful. Reading..." << endl;
    int docid, offset, size;
    while (ifs >> docid >> offset >> size)
    {
        _offsetLib[docid] = std::make_pair(offset, size);
    }

    cout << "[INFO] Read done. Offset lib size is " << _offsetLib.size() << endl; 
    ifs.close();
}

void WebPageQuery::loadIndexLib()
{
    string path = _conf.getConfig("indexlib");
    string name = "indexlib.dat";
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open \"" << name << ".dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Open \"" << name << ".dat\" successful. Reading..." << endl;
    string line;
    while (getline(ifs, line))
    {
        size_t pos_begin = line.find_first_of("\"");
        size_t pos_end = line.find_last_of("\"");
        string word = line.substr(pos_begin + 1, pos_end - 1);

        // remove ',', '{', '}'
        size_t pos = line.find_first_of("{");
        line = line.substr(pos + 2);

        pos = line.find_first_of(",");
        while (pos != string::npos)
        {
            line.replace(pos, 1, "");
            pos = line.find_first_of(",");
        }
        pos = line.find_last_of("}");
        line.replace(pos, 1, "");

        // get docid and weight
        pos_begin = line.find_first_not_of(" ");
        pos_end = line.find_first_of(" ");
        while (pos_end != string::npos)
        {
            size_t colon = line.find_first_of(":");
            int docid = std::stoi(line.substr(pos_begin, colon - pos_begin));
            double weight = std::stod(line.substr(colon + 1, pos_end - colon - 1));
            _invertIndexLib[word].push_back(std::make_pair(docid, weight)); 
            
            line = line.substr(pos_end + 1);
            pos_begin = line.find_first_not_of(" ");
            pos_end = line.find_first_of(" ");
        }
    }

    cout << "[INFO] Read done. Invert index lib size is " 
         << _invertIndexLib.size() << endl; 
    ifs.close();
}

int WebPageQuery::getTopk()
{
    return std::stoi(_conf.getConfig("search_topk"));
}

WebPage WebPageQuery::loadPage(int docid)
{
    string doc = findDoc(docid);    // 1. 栈对象
    WebPage page(doc);              // 2. WebPage(const string &) 
                                    //    doc以引用绑定（即 const string &）
                                    //    的方式传入构造
    return page;
}

string WebPageQuery::findDoc(int docid)
{
    // 根据 docid 去 _offsetLib 找到 (offset, size)
    auto it = _offsetLib.find(docid);
    if (it == _offsetLib.end())
    {
        return "";
    }

    int offset = it->second.first;
    int size = it->second.second;
    
    // 在 pagelib.dat 里 seekg(offset)
    // 读出一段原始 <doc>...</doc> 字符串
    ifstream ifs(_pageLib);
    if (!ifs.is_open())
    {
        return "";
    }

    ifs.seekg(offset);

    // 初始化size个'\0'到doc
    string doc(size, '\0');
    // 读取size个字符到doc
    ifs.read(&doc[0], size);

    return doc;
}

