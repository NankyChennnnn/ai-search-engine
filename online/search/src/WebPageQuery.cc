#include "WebPageQuery.h"
#include <cstdio>
#include <iostream>
#include <fstream>

// @TODO
// 1. 读取 pagelib.dat -> _pages
// 2. 读取 offsetlib.dat -> _offsetlib
// 3. 读取 indexlib.dat -> _invertIndexLib

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

WebPageQuery::WebPageQuery()
: _conf(Configuration::getInstance())
{
    loadOffsetLib();
    loadIndexLib();
    /* readDoc(docid); */
}

WebPageQuery &WebPageQuery::getInstance()
{
    static WebPageQuery instance;
    return instance;
}

string WebPageQuery::doQuery(const vector<string> &tokens)
{
    return "";
}

void WebPageQuery::loadOffsetLib()
{
    string path = _conf.getConfig("offsetlib");
    string name = "offsetlib.dat";
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open \"" << name << ".dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Open \"" << name << ".dat\" successful. Reading..." << endl;
    int docid, offset, size;
    while (ifs >> docid >> offset >> size)
    {
        _offsetLib[docid] = std::make_pair(offset, size);
    }

    cout << "[INFO] Read done. Offset lib size is " << _offsetLib.size() << endl; 
    ifs.close();
}

void WebPageQuery::loadIndexLib()
{
    string path = _conf.getConfig("indexlib");
    string name = "indexlib.dat";
    ifstream ifs(path);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open \"" << name << ".dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Open \"" << name << ".dat\" successful. Reading..." << endl;
    string line;
    while (getline(ifs, line))
    {
        size_t pos_begin = line.find_first_of("\"");
        size_t pos_end = line.find_last_of("\"");
        string word = line.substr(pos_begin + 1, pos_end - 1);

        // remove ',', '{', '}'
        size_t pos = line.find_first_of("{");
        line = line.substr(pos + 2);

        pos = line.find_first_of(",");
        while (pos != string::npos)
        {
            line.replace(pos, 1, "");
            pos = line.find_first_of(",");
        }
        pos = line.find_last_of("}");
        line.replace(pos, 1, "");

        // get docid and weight
        pos_begin = line.find_first_not_of(" ");
        pos_end = line.find_first_of(" ");
        while (pos_end != string::npos)
        {
            size_t colon = line.find_first_of(":");
            int docid = std::stoi(line.substr(pos_begin, colon - pos_begin));
            double weight = std::stod(line.substr(colon + 1, pos_end - colon - 1));
            _invertIndexLib[word].push_back(std::make_pair(docid, weight)); 
            
            line = line.substr(pos_end + 1);
            pos_begin = line.find_first_not_of(" ");
            pos_end = line.find_first_of(" ");
        }
    }

    cout << "[INFO] Read done. Invert index lib size is " 
         << _invertIndexLib.size() << endl; 
    ifs.close();
}

void WebPageQuery::readDoc(int docid)
{
    string path = _conf.getConfig("pagelib");
    string name = "pagelib.dat";
}
