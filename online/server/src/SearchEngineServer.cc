#include "SearchEngineServer.h"

using std::cout;
using std::endl;

SearchEngineServer::SearchEngineServer(const string &word)
: _keyRecommender(word)
{
}

SearchEngineServer::~SearchEngineServer()
{

}

void SearchEngineServer::start()
{
    vector<CandidateResult> results = _keyRecommender.doQuery();
    cout << "[INFO] Find candidate word";
    for (auto &cand : results)
    {
        cout << " [" << cand._word << "]";
    }
    cout << "." << endl;
}

void SearchEngineServer::stop()
{

}

void SearchEngineServer::onConnection()
{

}

void SearchEngineServer::onMessage()
{

}

void SearchEngineServer::onClose()
{

}
