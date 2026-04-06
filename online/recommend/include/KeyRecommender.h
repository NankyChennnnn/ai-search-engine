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

    string doQuery();

private:
    /* string _sought; */
    priority_queue<CandidateResult> _prique;
};

#endif
