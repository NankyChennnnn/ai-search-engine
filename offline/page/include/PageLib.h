#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include <utility>
#include "Configuration.h"
#include "DirScanner.h"
#include "FileProcessor.h"
#include "PageLibPreprocessor.h"
#include <unordered_map>
#include <set>

using std::unordered_map;
using std::set;
using std::pair;

class PageLib
{
public:
    PageLib();
    ~PageLib();

    void create(); // parse xml files
    void deduplication(); // page deduplication, use simhash
    void handleInvertIndex(); // create invert index lib
    void store();

private:
    string makeDoc(const RssItem &item, int docid);
    string escapeXml(const string &elem);

private:
    Configuration &_conf;
    const string DICT;
    const string HMM;
    const string USER;
    const string IDF;
    const string STOP_WORD;
    cppjieba::Jieba _jieba;
    
private:
    vector<RssItem> _rss;
    DirScanner _dirScanner;
    FileProcessor _fileProcessor;
    PageLibPreprocessor _plPreprocessor;
};

#endif
