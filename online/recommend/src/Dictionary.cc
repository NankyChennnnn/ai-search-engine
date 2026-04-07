#include "Dictionary.h"
#include <cstddef>
#include <iostream>
#include <fstream>
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

vector<pair<string, int>> Dictionary::doQuery(const string &key)
{
    vector<pair<string, int>> results;
    
    // 按字符取候选id
    set<int> canids = getCandidateIds(key);

    // 根据索引找所有候选词
    results = queryIndex(canids);
    return results;
}

int Dictionary::getTopk()
{
    return std::stoi(_conf.getConfig("topk"));
}

void Dictionary::createDict()
{
    string name = "en_dict";
    string path = _conf.getConfig(name);
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
        _dict.push_back(std::make_pair(word, freq));
    }

    ifs.close();
    cout << "[INFO] Scan done." << endl;
}

void Dictionary::createIndex()
{
    string name = "en_index";
    string path = _conf.getConfig(name);
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        throw std::runtime_error("[ERROR] Open index lib \"" + name + ".dat\" failed.");
    }

    cout << "[INFO] Open index lib successful. Scanning..." << endl;
    string line;
    while (getline(ifs, line))
    {
        size_t pos = line.find("\"");
        string word(1, line[pos + 1]);

        // remove ',', '{', '}'
        pos = line.find_first_of("{");
        line = line.substr(pos + 2);

        pos = line.find_first_of(",");
        while (pos != string::npos)
        {
            line.replace(pos, 1, "");
            pos = line.find_first_of(",");
        }
        pos = line.find_last_of("}");
        line.replace(pos, 1, "");

        size_t pos_begin = line.find_first_not_of(" ");
        size_t pos_end = line.find_first_of(" ");
        while (pos_end != string::npos)
        {
            _index[word].insert(std::stoi(line.substr(pos_begin, pos_end)));
            line = line.substr(pos_end + 1);
            pos_begin = line.find_first_not_of(" ");
            pos_end = line.find_first_of(" ");
        }
    }

    ifs.close();
    cout << "[INFO] Scan done." << endl;
}

set<int> Dictionary::getCandidateIds(const string &key)
{
    // 拆分key成单个字母
    // 遍历index，拿到每个字母的所有索引
    set<int> canids;
    for (auto &k : key)
    {
        string letter(1, k);
        auto it = _index.find(letter);
        if (it != _index.end())
        {
            canids.insert(it->second.begin(), it->second.end());
        }
    }
    return canids;
}

vector<pair<string, int>> Dictionary::queryIndex(const set<int> &canids)
{
    vector<pair<string, int>> results;
    for (auto id : canids)
    {
        if (id > 0 && static_cast<size_t>(id) <= _dict.size())
        {
            results.push_back(_dict[id - 1]); 
        }
    }
    return results;
}

Dictionary::Dictionary()
: _conf(Configuration::getInstance())
{
    createDict();
    createIndex();
}
