#include "PageLib.h"
#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

using std::cout;
using std::endl;
using std::ofstream;

PageLib::PageLib()
: _conf(Configuration::getInstance())
, _jieba(_conf.getConfig("dict"),
         _conf.getConfig("hmm"),
         _conf.getConfig("user"),
         _conf.getConfig("idf"),
         _conf.getConfig("stop_word"))
{
    // store all xml files
    _dirScanner.traverse(_conf.getConfig("pdo_xml_dir"));

    // parse xml files, use tinyxml2
    create();

    // complete page lib
    deduplication();
    
    // create invert index lib
    handleInvertIndex();
    
    // store all dat files
    store();
}

PageLib::~PageLib()
{

}

void PageLib::create()
{
    cout << "[INFO] Origin page lib creating..." << endl;
    vector<string> &files = _dirScanner.getFiles();
    for (const auto &file : files)
    {
        _fileProcessor.process(file, _rss);
    }

    cout << "[INFO] Origin page lib create successful. " 
         << "Size is " << _rss.size() << "." << endl;
}

void PageLib::deduplication()
{
    cout << "[INFO] Page deduplicating..." << endl;
    simhash::Simhasher simhasher(_conf.getConfig("dict"),
                                 _conf.getConfig("hmm"),
                                 _conf.getConfig("idf"),
                                 _conf.getConfig("stop_word"));
    _plPreprocessor.cutRedundantPage(_rss, simhasher);

    cout << "[INFO] Page deduplication successful. " 
         << "Deduplicated page lib size is " << _rss.size() << "." << endl;
}

void PageLib::handleInvertIndex()
{
    cout << "[INFO] Invert index lib building..." << endl;
    _plPreprocessor.buildInvertIndexMap(_rss, _jieba);
}

void PageLib::store()
{
    string output_dir = _conf.getConfig("output_dir");
    struct stat st;
    if (stat((output_dir).c_str(), &st) == 0 &&
        S_ISDIR(st.st_mode))
    {
        cout << "[INFO] Find output dir \"" << output_dir << "\"." << endl;
    }
    else
    {
        mkdir((output_dir).c_str(), 0755);
        cout << "[INFO] Create output dir \"" << output_dir << "\"." << endl;
    }

    cout << "[INFO] Storing \"pagelib.dat\", \"offsetlib.dat\" and \"indexlib.dat\""
         << " to \"" << output_dir << "\"..." << endl;
    
    string pagelib = output_dir + "pagelib.dat";
    string offsetlib = output_dir + "offsetlib.dat";
    string indexlib = output_dir + "indexlib.dat";
    
    ofstream ofs_pagelib(pagelib);
    ofstream ofs_offsetlib(offsetlib);
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

    _plPreprocessor.storeOnDisk(indexlib);

    cout << "[INFO] Store .dat files done."<< endl;
}

string PageLib::makeDoc(const RssItem &item, int docid)
{
    std::ostringstream oss;
    oss << "<doc>\n" 
        << "\t<docid>" << docid << "</docid>\n"
        << "\t<title>" << escapeXml(item.title) << "</title>\n"
        << "\t<link>" << escapeXml(item.link) << "</link>\n"
        << "\t<description>" << escapeXml(item.description) << "</description>\n"
        << "\t<content>" << escapeXml(item.content) << "</content>\n"
        << "</doc>\n";
    return oss.str();
}

string PageLib::escapeXml(const string &elem)
{
    string result;
    for (char ch : elem)
    {
        switch (ch)
        {
        case '&':
            result += "&amp;";
            break;
        case '<':
            result += "&lt;";
            break;
        case '>':
            result += "&gt;";
            break;
        case '\"':
            result += "&quot;";
            break;
        case '\'':
            result += "&apos;";
            break;
        default:
            result += ch;
            break;
        }
    }
    return result;
}
