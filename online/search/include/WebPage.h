#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

/* 解析单个 <doc> 字符串，拿到 content*/
#include <string>

using std::string;

struct Page
{
    int docid;
    string title;
    string link;
    string description;
    string content;
};

class WebPage
{
public:
    WebPage(const string &doc);
    ~WebPage();
    int getDocid();
    string getTitle();
    string getLink();
    string getDescription();
    string getContent();

private:
    string getTagContent(const string &tag);

private:
    string _doc;
    Page _page;
};

#endif
