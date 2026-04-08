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
    WebPage();
    ~WebPage();

private:
    Page _page;
};

#endif
