#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "Configuration.h"
#include <initializer_list>
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

enum class QueryType {
    EN,
    ZH,
    INVALID
};

struct RecommendLib {
    vector<pair<string, int>> _dict; // <word, freq>
    map<string, set<int>> _index;
};

class Dictionary
{
public:
    static Dictionary &getInstance();
    vector<pair<string, int>> doQuery(const vector<string> &tokens, QueryType type);
    int getTopk();

private:
    void createDict(std::initializer_list<string> names);
    void createIndex(std::initializer_list<string> names);
    set<int> getCandidateIds(const vector<string> &tokens, QueryType type);
    vector<pair<string, int>> queryIndex(const set<int> &canids, QueryType type);
    RecommendLib &getRecommendLib(const string &name);
    RecommendLib &getRecommendLib(QueryType type);

private:
    Dictionary();
    ~Dictionary() = default;
    Dictionary(const Dictionary &) = delete;
    Dictionary &operator=(const Dictionary &) = delete;

private:
    Configuration &_conf;
    RecommendLib _engLib;
    RecommendLib _zhLib;
};

#endif
