#ifndef __SEARCHENGINESERVER_H__
#define __SEARCHENGINESERVER_H__

#include "ThreadPool.h"
#include "KeyRecommender.h"
#include "WebPageSearcher.h"
#include "ProtocolParser.h"
#include <string>

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
    // 在线程池任务里临时创建task对象
    /* void doTaskThread(const TcpConnectionPtr &ptr, string &msg); */

private:
    ThreadPool _threadPool;
    /* TcpServer _tcpServer; */
    KeyRecommender _keyRecommender;
    WebPageSearcher _webPageSearcher;
    ProtocolParser _protocolParser;
};

#endif
