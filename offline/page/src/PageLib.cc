#include "PageLib.h"
#include "Configuration.h"
#include "tinyxml2/tinyxml2.h"
#include <iostream>
#include <regex>
#include <set>
#include <fstream>
#include <utility>

using std::cout;
using std::endl;
using namespace tinyxml2;
using std::regex;
using std::regex_replace;
using std::ofstream;

PageLib::PageLib()
: _conf(Configuration::getInstance())
, _jieba(_conf.getConfig("dict"),
         _conf.getConfig("hmm"),
         _conf.getConfig("user"),
         _conf.getConfig("idf"),
         _conf.getConfig("stop_word"))
{
    _dirScanner.traverse(_conf.getConfig("pdo_xml_dir")); // store all xml files

    create();   // parse xml files, use tinyxml2
    deduplication(); // complete page lib
    handleInvertIndex(); // create invert index lib
    store(); // store all dat files
}

PageLib::~PageLib()
{

}

void PageLib::create()
{
    cout << "[INFO] Page lib infomation collecting..." << endl;
    vector<string> &files = _dirScanner.getFiles();
    XMLDocument doc;
    for (const auto &file : files)
    {
        doc.LoadFile(file.c_str());
        size_t name_pos = file.find_last_of('/');
        string name = file.substr(name_pos + 1);
        if (doc.ErrorID())
        {
            cerr << "[ERROR] XML file \"" << name << "\" parse failed." << endl;
            return;
        }

        XMLElement *itemNode = doc.FirstChildElement("rss")
            ->FirstChildElement("channel")
            ->FirstChildElement("item");
        while (itemNode)
        {
            XMLElement *titleElem = itemNode->FirstChildElement("title");
            XMLElement *linkElem = itemNode->FirstChildElement("link");
            XMLElement *descriptionElem = itemNode->FirstChildElement("description");
            XMLElement *contentElem = itemNode->FirstChildElement("content");
            XMLElement *encodedElem = itemNode->FirstChildElement("content:encoded");

            string title, link, description, content; 
            if (titleElem)
            {
                title = titleElem->GetText();
            }

            if (linkElem)
            {
                link = linkElem->GetText();
            }

            if (descriptionElem)
            {
                description = descriptionElem->GetText();
            }

            if (contentElem)
            {
                content = contentElem->GetText();
            }
            else if (encodedElem)
            {
                content = encodedElem->GetText();
            }

            regex rgx("<[^>]+>"); // find <...></...>
            description = regex_replace(description, rgx, ""); // replace "<...>" to ""
            content = regex_replace(content, rgx, "");

            RssItem ri;
            ri.title = title;
            ri.link = link;
            ri.description = description;
            ri.content = content;

            _rss.push_back(ri);

            itemNode = itemNode->NextSiblingElement("item"); // find next item
        }
    }

    cout << "[INFO] Information collection successful. " 
         << "Origin infomation size is " << _rss.size() << "." << endl;
}

void PageLib::deduplication()
{
    simhash::Simhasher simhasher(_conf.getConfig("dict"),
                                 _conf.getConfig("hmm"),
                                 _conf.getConfig("idf"),
                                 _conf.getConfig("stop_word"));
    _plPreprocessor.cutRedundantPage(_rss, simhasher);

    cout << "[INFO] Information deduplication successful. " 
         << "Deduplicated infomation size is " << _rss.size() << "." << endl;
}

void PageLib::handleInvertIndex()
{
    _plPreprocessor.buildInvertIndexMap(_rss, _invertIndex, _jieba);

    cout << "[INFO] Create invert index lib successful. " 
         << "Invert index lib size is " << _invertIndex.size() << "." << endl;
}

void PageLib::store()
{
    string output_dir = _conf.getConfig("output_dir");
    string pagelib_fileName = output_dir + "/" + "pagelib.dat";
    string offsetlib_fileName = output_dir + "/" + "offsetlib.dat";
    ofstream ofs_pagelib(pagelib_fileName);
    ofstream ofs_offsetlib(offsetlib_fileName);
    if (!ofs_pagelib.is_open())
    {
        cerr << "[ERROR] Open \"pagelib.dat\" failed." << endl;
        return;
    }

    if (!ofs_offsetlib.is_open())
    {
        cerr << "[ERROR] Open \"offsetlib.dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Writing \"pagelib.dat\" and \"offsetlib.dat\" to \"" 
         << output_dir << "\"..." << endl;

    int docid = 1;
    int doc_offset = 0;
    int docsz = 0;
    for (auto it = _rss.begin(); it != _rss.end(); ++it, ++docid)
    {
        string doc = makeDoc(*it, docid);
        ofs_pagelib << doc;
        doc_offset += docsz;
        docsz = doc.size();
        ofs_offsetlib << docid << " " << doc_offset << " " << docsz << endl;
    }

    cout << "[INFO] Write \"pagelib.dat\" and \"offsetlib.dat\" done."<< endl;

    string indexlib_fileName = output_dir + "/" + "indexlib.dat";
    ofstream ofs_indexlib(indexlib_fileName);
    if (!ofs_indexlib.is_open())
    {
        cerr << "[ERROR] Open \"indexlib.dat\" failed." << endl;
        return;
    }

    cout << "[INFO] Writing \"indexlib.dat\" to \"" 
         << output_dir << "\"..." << endl;

    for (auto it = _invertIndex.begin(); it != _invertIndex.end(); ++it, ++docid)
    {
        bool first = true;
        string word = it->first;
        ofs_indexlib << "\"" << word << "\" : {";
        for (auto num = it->second.begin(); num != it->second.end(); ++num)
        {
            if (first)
            {
                ofs_indexlib << " " << num->first << ":" << num->second;
                first = false;
            }
            else
            {
                ofs_indexlib << ", " << num->first << ":" << num->second;
            }
        }
        ofs_indexlib << " }" << endl;
    }

    cout << "[INFO] Write \"indexlib.dat\" done."<< endl;
}

string PageLib::makeDoc(RssItem item, int docid)
{
    std::ostringstream oss;
    oss << "<doc>\n" 
        << "\t<docid>" << docid << "</docid>\n"
        << "\t<title>" << item.title << "</title>\n"
        << "\t<link>" << item.link << "</link>\n"
        << "\t<description>" << item.description << "</description>\n"
        << "\t<content>" << item.content << "</content>\n"
        << "</doc>\n";
    return oss.str();
}
