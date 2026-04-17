#ifndef __PROTOCOLPARSER_H__
#define __PROTOCOLPARSER_H__

#include <string>

using std::string;

struct Request
{
    int type; // 0-Recommender, 1-WebPageSearcher
    string query;
};

class ProtocolParser
{
public:
    ProtocolParser();
    ~ProtocolParser();
    Request doParse(const string &msg);

private:
    string _msg;
};

#endif
