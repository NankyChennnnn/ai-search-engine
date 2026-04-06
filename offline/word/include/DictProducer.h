#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "WordConfig.h"
#include "WordSegmentation.h"
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_set;
using std::unordered_map;
using std::ifstream;
using std::ofstream;

class DictProducer
{
public:
    DictProducer(WordConfig wordConf);
    ~DictProducer();
    
    // 创建词典
    void createEnDict();
    void createCnDict();
    
    // 创建索引库
    void createEnIndex();
    void createCnIndex();

    // 保存dat文件
    void store();

private:
    // 去停用词
    bool isStopWord(const string &word);

    // 文件处理
    void saveStopWords(const string &fileName);
    void processEnLine(string &line);
    void processCnLine(string &line);
    void openEnFile(ifstream &ifs, const string &fileName);
    void openCnFile(ifstream &ifs, const string &fileName);
    void inputFile(ofstream &ofs, const string &fileName);
    
    // 单词处理
    string dealEnWord(const string &word);
    void insertToEnMap(const string &word);
    bool isValidWord(const string &word);
    void insertToCnMap(const string &word);

private:
    // 输入的文件列表
    vector<string> _enFiles;
    vector<string> _cnFiles;

    // 词频
    unordered_map<string, int> _enDict;
    unordered_map<string, int> _cnDict;
    
    // 用字母找单词ID
    unordered_map<string, unordered_set<int>> _enIndex;
    unordered_map<string, unordered_set<int>> _cnIndex;
    
    // 用ID找单词
    unordered_map<int, string> _enIdMap;
    unordered_map<int, string> _cnIdMap;
    
    // ID统计
    int _enIdCounter;
    int _cnIdCounter;

    // 停用词集合
    unordered_set<string> _stopWords;

    WordSegmentation _wordSeg;
    WordConfig _wordConf;
};

#endif
