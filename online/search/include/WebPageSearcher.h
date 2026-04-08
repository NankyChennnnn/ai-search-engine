#ifndef __WEBPAGESEARCHER_H__
#define __WEBPAGESEARCHER_H__

/* 接收原始查询串，调用 WebPageQuery，组织输出 */

#include <string>
#include "SplitTool.h"

using std::string;

class WebPageSearcher
{
public:
    WebPageSearcher(const string &word);
    ~WebPageSearcher();

    string doQuery();

private:
    string _sought;
    SplitTool &_tool;
    /* TcpConnectionPtr _conn; */
};

#endif
