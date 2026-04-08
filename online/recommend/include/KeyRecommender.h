#ifndef __KEYRECOMMENDER_H__
#define __KEYRECOMMENDER_H__

#include "Dictionary.h"
#include "CandidateResult.h"
#include <queue>

using std::priority_queue;

class KeyRecommender
{
public:
    KeyRecommender(const string &word);
    ~KeyRecommender();

    vector<CandidateResult> doQuery();

private:
    int distance(const vector<string> &word, const vector<string> &cand);
    QueryType detectQueryType(const string &query);
    vector<string> getTokens(const string &query, QueryType type);

private:
    string _sought;
    priority_queue<CandidateResult> _prique;
};

#endif
