#include "FileProcessor.h"
#include <stdexcept>
#include <regex>

using std::regex;
using std::regex_replace;

FileProcessor::FileProcessor()
{

}

FileProcessor::~FileProcessor()
{

}

void FileProcessor::process(const string &fileName, vector<RssItem> &rss)
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
        title = getText(titleElem);
        link = getText(linkElem);
        description = getText(descriptionElem);

        if (contentElem)
        {
            content = getText(contentElem);
        }
        else if (encodedElem)
        {
            content = getText(encodedElem);
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

string FileProcessor::getText(XMLElement *elem)
{
    if (elem)
    {
        if (elem->GetText() != nullptr)
        {
            string text = elem->GetText();
            size_t pos = 0;
            while ((pos = text.find("&nbsp;"), pos) != string::npos)
            {
                text.replace(pos, 6, " ");
            }

            return text;
        }
    }
    return "";
}
