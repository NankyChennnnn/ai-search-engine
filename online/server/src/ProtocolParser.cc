#include "ProtocolParser.h"

ProtocolParser::ProtocolParser()
{

}

ProtocolParser::~ProtocolParser()
{

}

Request ProtocolParser::doParse(const string &msg)
{
    Request req = {-1, ""};
    if (msg.empty()) return req;

    size_t pos = msg.find(' ');
    if (pos == string::npos) return req;

    try {
        req.type = std::stoi(msg.substr(0, pos));

        string m = msg.substr(pos + 1);
        pos = m.find_first_not_of(' ');
        if (pos == string::npos) return {-1, ""};
        req.query = m.substr(pos);
    } catch (...) {
        return {-1, ""};
    }

    if (req.query.empty())
    {
        return {-1, ""};
    }

    return req;
}
