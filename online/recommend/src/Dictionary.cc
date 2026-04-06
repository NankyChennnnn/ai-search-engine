#include "Dictionary.h"
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

vector<string> Dictionary::doQuery(const string &key)
{
    vector<string> results;
    for (auto &word : _dict)
    {
        size_t pos = word.first.find(key);
        if (pos != string::npos)
        {
            results.push_back(word.first);
        }
    }
    return results;
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
    cout << "[INFO] Scan done." << endl;
    ifs.close();
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

void Dictionary::queryIndex()
{

}

int Dictionary::distance(string candidate)
{
    return 0;
}

Dictionary::Dictionary()
: _conf(Configuration::getInstance())
{
    createDict();
    createIndex();
}
