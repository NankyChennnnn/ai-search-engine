#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include <string>
#include <vector>
#include "RssItem.h"

using std::string;
using std::vector;

class FileProcessor
{
public:
    FileProcessor();
    ~FileProcessor();

    void process(const string fileName, vector<RssItem> &rss);
};

#endif
