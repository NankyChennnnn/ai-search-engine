#include "Dictionary.h"
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;

Dictionary &Dictionary::getInstance()
{
    static Dictionary instance;
    return instance;
}

vector<pair<string, int>> Dictionary::doQuery(const vector<string> &tokens,
                                              QueryType type)
{
    cout << "[INFO] Finding candidate words..." << endl;
    vector<pair<string, int>> results;
    
    // 按字符取候选id
    set<int> canids = getCandidateIds(tokens, type);

    // 根据索引找所有候选词
    results = queryIndex(canids, type);
    cout << "[INFO] Find done." << endl;
    return results;
}

int Dictionary::getTopk()
{
    return std::stoi(_conf.getConfig("topk"));
}

void Dictionary::createDict(std::initializer_list<string> names)
{
    for (const auto &name : names)
    {
        string path = _conf.getConfig(name);
        RecommendLib &lib = getRecommendLib(name);
        ifstream ifs(path);
        if (!ifs.is_open())
        {
            throw std::runtime_error("[ERROR] Open dictionary \"" + name + ".dat\" failed.");
        }

        cout << "[INFO] Open dictionary successful. Scanning..." << endl;
        string line;
        while (getline(ifs, line))
        {
            size_t pos = line.find("    ");
            string word = line.substr(0, pos);
            int freq = std::stoi(line.substr(pos + 4));
            lib._dict.push_back(std::make_pair(word, freq));
        }

        ifs.close();
        cout << "[INFO] Scan done. Dictionary size is " << lib._dict.size() << "." << endl;
    }
}

void Dictionary::createIndex(std::initializer_list<string> names)
{
    for (const auto &name : names)
    {
        string path = _conf.getConfig(name);
        RecommendLib &lib = getRecommendLib(name);
        ifstream ifs(path);
        if (!ifs.is_open())
        {
            throw std::runtime_error("[ERROR] Open index lib \"" + name + ".dat\" failed.");
        }

        cout << "[INFO] Open index lib successful. Scanning..." << endl;
        string line;
        while (getline(ifs, line))
        {
            size_t pos_begin = line.find_first_of("\"");
            size_t pos_end = line.find_last_of("\"");
            string word = line.substr(pos_begin + 1, pos_end - pos_begin - 1);

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

            pos_begin = line.find_first_not_of(" ");
            pos_end = line.find_first_of(" ");
            while (pos_end != string::npos)
            {
                lib._index[word].insert(std::stoi(line.substr(pos_begin, 
                                                              pos_end - pos_begin)));
                line = line.substr(pos_end + 1);
                pos_begin = line.find_first_not_of(" ");
                pos_end = line.find_first_of(" ");
            }
        }

        ifs.close();
        cout << "[INFO] Scan done. Index size is " << lib._index.size() << "." << endl;
    }
}

set<int> Dictionary::getCandidateIds(const vector<string> &tokens, QueryType type)
{
    // 拆分key成单个字母
    // 遍历index，拿到每个字母的所有索引
    RecommendLib &lib = getRecommendLib(type);
    set<int> canids;
    for (auto &token : tokens)
    {
        auto it = lib._index.find(token);
        if (it != lib._index.end())
        {
            canids.insert(it->second.begin(), it->second.end());
        }
    }
    return canids;
}

vector<pair<string, int>> Dictionary::queryIndex(const set<int> &canids, QueryType type)
{
    RecommendLib &lib = getRecommendLib(type);
    vector<pair<string, int>> results;
    for (auto id : canids)
    {
        if (id > 0 && static_cast<size_t>(id) <= lib._dict.size())
        {
            results.push_back(lib._dict[id - 1]); 
        }
    }
    return results;
}

RecommendLib &Dictionary::getRecommendLib(const string &name)
{
    if (name == "en_dict" || name == "en_index") return _engLib;
    if (name == "cn_dict" || name == "cn_index") return _zhLib;
    throw std::runtime_error("[ERROR] Invalid file name.");
}

RecommendLib &Dictionary::getRecommendLib(QueryType type)
{
    if (type == QueryType::EN) return _engLib;
    if (type == QueryType::ZH) return _zhLib;
    throw std::runtime_error("[ERROR] Invalid type.");
}

Dictionary::Dictionary()
: _conf(Configuration::getInstance())
{
    createDict({"en_dict", "cn_dict"});
    createIndex({"en_index", "cn_index"});
}
