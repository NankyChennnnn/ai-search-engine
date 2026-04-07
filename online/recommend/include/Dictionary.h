#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "Configuration.h"
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <set>

using std::vector;
using std::pair;
using std::string;
using std::map;
using std::set;

class Dictionary
{
public:
    static Dictionary &getInstance();
    vector<pair<string, int>> doQuery(const string &key);
    int getTopk();

private:
    void createDict();
    void createIndex();
    set<int> getCandidateIds(const string &key);
    vector<pair<string, int>> queryIndex(const set<int> &canids);

private:
    Dictionary();
    ~Dictionary() = default;
    Dictionary(const Dictionary &) = delete;
    Dictionary &operator=(const Dictionary &) = delete;

private:
    Configuration &_conf;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
};

#endif
