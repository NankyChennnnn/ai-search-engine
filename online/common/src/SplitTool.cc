#include "SplitTool.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::cout;
using std::endl;

SplitTool::SplitTool()
: _conf(Configuration::getInstance())
, _jieba(_conf.getConfig("dict"),
         _conf.getConfig("hmm"),
         _conf.getConfig("user"),
         _conf.getConfig("idf"),
         _conf.getConfig("stop_word"))
{
    buildStopWordsMap();
}

SplitTool &SplitTool::getInstance()
{
    static SplitTool instance;
    return instance;
}

void SplitTool::buildStopWordsMap()
{
    string path = _conf.getConfig("stop_words_zh");
    ifstream ifs(path);
    size_t pos = path.find_last_of('/');
    string name = path.substr(pos + 1);
    if (!ifs.is_open())
    {
        throw std::runtime_error("[ERROR] Open file \"" + name + "\" failed!");
    }

    cout << "[INFO] Open file \"" << name << "\" successfully. Reading..." << endl;

    // 默认停用词库文件单行一个单词
    string word;
    while (getline(ifs, word))
    {
        // 清除首位空白内容
        word.erase(0, word.find_first_not_of(" \t\n\r"));
        word.erase(word.find_last_not_of(" \t\n\r") + 1);

        // 跳过空行和注释
        if (word.empty() || word[0] == '#') continue;

        _stopWords.insert(word);
    }

    cout << "[INFO] Read done." << endl;
    ifs.close();
}

vector<string> SplitTool::cutAndFilter(const string &text)
{
    vector<string> raw, results;
    _jieba.CutAll(text, raw);
    for (auto &word : raw)
    {
        if (!isValidWord(word)) continue;
        if (isStopWord(word)) continue;

        results.push_back(word);
    }

    return results;
}

vector<string> SplitTool::decodeRunesInString(const string &query)
{
    vector<string> tokens;

    cppjieba::RuneStrArray runes;
    if (!cppjieba::DecodeRunesInString(query, runes))
    {
        return tokens;
    }

    tokens.reserve(runes.size());
    for (const auto &r : runes)
    {
        tokens.push_back(query.substr(r.offset, r.len));
    }

    return tokens;
}

bool SplitTool::isValidWord(const string &word)
{
    // 空串过滤
    if (word.empty()) return false;
    
    // 按 UTF-8 拆分字符，返回false则无法正确拆分，字符有问题，丢弃
    cppjieba::RuneStrArray runes;
    if (!cppjieba::DecodeRunesInString(word, runes))
    {
        return false;
    }

    // 拆解成功后，遍历字符数组
    for (const auto &rs : runes)
    {
        // 常用汉字范围，在内保留
        if (rs.rune >= 0x4E00 && rs.rune <= 0x9FFF) return true;

        // 字符在ASCII范围内，保留
        if (rs.rune < 0x80 && std::isalnum(static_cast<unsigned char>(rs.rune)))
        {
            return true;
        }
    }

    return false;
}

bool SplitTool::isStopWord(const string &word)
{
    for (auto &stopWord : _stopWords)
    {
        if (word == stopWord)
        {
            return true;
        }
    }

    return false;
}
