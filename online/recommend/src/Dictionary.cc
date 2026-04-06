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
    string name = "en_dict";
    string path = _conf.getConfig(name);
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        throw std::runtime_error("[ERROR] Open dictionary \"" + name + "\" failed.");
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
}
