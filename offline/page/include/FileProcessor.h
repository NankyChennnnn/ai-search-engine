#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include <string>
#include <vector>
#include "XmlReader.h"
#include "tinyxml2/tinyxml2.h"

using std::string;
using std::vector;
using namespace tinyxml2;

class FileProcessor
{
public:
    FileProcessor();
    ~FileProcessor();

    void process(const string &fileName, vector<RssItem> &rss);

private:
    string getText(XMLElement *elem);
};

#endif
