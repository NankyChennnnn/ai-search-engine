#include "WebPageSearcher.h"
#include "WebPage.h"
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
    vector<WebPage> pages = query.doQuery(tokens);

    int i = 1;
    for (auto &page : pages)
    {
        cout << "[Result " << i++ << "]" << endl;
        cout << "Title: " << page.getTitle() << endl;
        cout << "Link : " << page.getLink() << endl;

        string desc = page.getDescription();
        if (desc.empty())
        {
            desc = truncateUTF8(page.getContent(), 20);
        }
        else
        {
            desc = truncateUTF8(desc, 20);
        }
        cout << "Desc : " << desc << endl;
    }
    return "[WARNING] WebPageSearcher Test done.";
}

string WebPageSearcher::truncateUTF8(const string &text, size_t count)
{
    string result;
    vector<string> chars = _tool.decodeRunesInString(text);
    size_t limit = std::min(count, chars.size());
    for (size_t i = 0; i < limit; ++i)
    {
        result += chars[i];
    }

    if (chars.size() > count) result += "...";

    return result;
}
