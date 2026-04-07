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
    int distance(const string &word, const string &cand);

private:
    string _sought;
    priority_queue<CandidateResult> _prique;
};

#endif
