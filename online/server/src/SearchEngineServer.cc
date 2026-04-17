#include "SearchEngineServer.h"

using std::cout;
using std::endl;

SearchEngineServer::SearchEngineServer(const string &word)
: _threadPool(4, 10)
, _keyRecommender(word)
, _webPageSearcher(word)
{
    Request r = _protocolParser.doParse(word);
    cout << "[WARNING] Protocol Parser --> " 
        << r.type << ", " << r.query << std::endl;
}

SearchEngineServer::~SearchEngineServer()
{

}

void SearchEngineServer::start()
{
    _threadPool.start();
    /* vector<CandidateResult> results = _keyRecommender.doQuery(); */
    /* cout << "[INFO] Find candidate word"; */
    /* for (auto &cand : results) */
    /* { */
    /*     cout << " [" << cand._word << "]"; */
    /* } */
    /* cout << "." << endl; */

    cout << "[WARNING] Test WebPageSearcher" << endl
         << _webPageSearcher.doQuery() << endl;
}

void SearchEngineServer::stop()
{

}

void SearchEngineServer::onConnection()
{

}

void SearchEngineServer::onMessage()
{
    // 收到一条请求消息
    // 解析协议
    // 拿到 type 和 query
    // 请求一个任务
}

void SearchEngineServer::onClose()
{

}
