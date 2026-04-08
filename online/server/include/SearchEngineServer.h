#ifndef __SEARCHENGINESERVER_H__
#define __SEARCHENGINESERVER_H__

#include "KeyRecommender.h"
#include <string>
#include "WebPageSearcher.h"

using std::string;

class SearchEngineServer
{
public:
    SearchEngineServer(const string &word);
    ~SearchEngineServer();

    void start();
    void stop();
    void onConnection();
    void onMessage();
    void onClose();
    /* void doTaskThread(const TcpConnectionPtr &ptr, string &msg); */

private:
    /* Threadpool _threadPool; */
    /* TcpServer _tcpServer; */
    KeyRecommender _keyRecommender;
    WebPageSearcher _webPageSearcher;
    /* ProtocolParser _protocolParser; */
};

#endif
