#include "WebPageSearcher.h"
#include "WebPageQuery.h"
#include <iostream>

using std::cout;
using std::endl;

WebPageSearcher::WebPageSearcher(const string &word)
: _sought(word)
, _tool(SplitTool::getInstance())
{

}

WebPageSearcher::~WebPageSearcher()
{

}

string WebPageSearcher::doQuery()
{
    WebPageQuery &query = WebPageQuery::getInstance();    
    vector<string> tokens = _tool.cutAndFilter(_sought);
    return query.doQuery(tokens);
}
