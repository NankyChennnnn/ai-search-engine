#include "WebPage.h"
#include <string>

WebPage::WebPage(const string &doc)
: _doc(doc) // 3. 如果 _doc 为 const string &，此时进行引用绑定
            //    _doc 指向 loadPage 里的局部变量 doc
            // 4. 构造函数期间调用 getTagContent，能够正常工作
            // 5. 但是 loadPage 返回后，doc 被销毁，_doc就变成了
            //    悬空引用，此时再调用find(), substr()就会有几率崩溃
            // 6. 所以_doc要改成 string，使底层完成深拷贝
            //    就不会出现悬空引用了
{
    string docid;
    docid = getTagContent("docid");
    _page.docid = docid.empty() ? 0 : std::stoi(docid);
    
    _page.title = getTagContent("title"); 
    _page.link = getTagContent("link");
    _page.description = getTagContent("description"); 
    _page.content = getTagContent("content");
}

WebPage::~WebPage()
{
}

string WebPage::getTagContent(const string &tag)
{
    string begin = "<" + tag + ">";
    string end = "</" + tag + ">";
    size_t bsize = begin.size();

    string text = "";
    size_t bpos = _doc.find(begin);
    size_t epos = _doc.find(end);
    if (bpos == string::npos || epos == string::npos)
    {
        return text;
    }

    if (epos < bpos + bsize)
    {
        return text;
    }

    text = _doc.substr(bpos + bsize, epos - bpos - bsize);
    return text;
}

int WebPage::getDocid()
{
    return _page.docid;
}

string WebPage::getTitle()
{
    return _page.title;
}

string WebPage::getLink()
{
    return _page.link;
}

string WebPage::getDescription()
{
    return _page.description;
}

string WebPage::getContent()
{
    return _page.content;
}
