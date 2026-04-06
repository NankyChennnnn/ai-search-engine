#include "FileProcessor.h"
#include <stdexcept>
#include <regex>
#include "RssItem.h"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;
using std::regex;
using std::regex_replace;

FileProcessor::FileProcessor()
{

}

FileProcessor::~FileProcessor()
{

}

void FileProcessor::process(const string fileName, vector<RssItem> &rss)
{
    XMLDocument doc;
    doc.LoadFile(fileName.c_str());
    size_t name_pos = fileName.find_last_of('/');
    string name = fileName.substr(name_pos + 1);
    if (doc.ErrorID())
    {
        throw std::runtime_error("[ERROR] XML file \"" + name + "\" parse failed." );
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

        rss.push_back(ri);
        
        itemNode = itemNode->NextSiblingElement("item"); // find next item
    }
}
