#ifndef __XMLREADER_H__
#define __XMLREADER_H__

#include "PageLib.h"
#include <string>

using std::string;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class XmlReader
{
public:
    XmlReader();
    ~XmlReader();

private:

};

#endif
