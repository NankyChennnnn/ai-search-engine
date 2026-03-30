#include "DictProducer.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

using std::cout;
using std::endl;
using std::cerr;
using std::istringstream;

DictProducer::DictProducer()
: _idCounter(0)
{
    saveStopWords("stop_words_eng.txt");
    saveStopWords("stop_words_zh.txt");
    _enFiles.push_back("english.txt");
    _enFiles.push_back("The_Holy_Bible.txt");

    string dirname = "../data/CN/art/";
    DIR* dir;
    struct dirent* direntptr;
    dir = opendir(dirname.c_str());
    if (!dir)
    {
        cout << "This " << dirname.c_str() << " is null." << endl;
        return;
    }

    while((direntptr = readdir(dir)) != nullptr)
    {
        // 去掉当前目录和上一级目录，避免死循环
        if (strncmp(direntptr->d_name, ".", 1) != 0 &&
            strncmp(direntptr->d_name, "..", 2) != 0)
        {
            _cnFiles.push_back(direntptr->d_name);
        }
    }
}

DictProducer::~DictProducer()
{

}

void DictProducer::createEnDict()
{
    cout << "Start creating English dictionary..." << endl;
    for (const auto &file : _enFiles)
    {
        // 防止文件类型不同
        ifstream ifs;
        openEnFile(ifs, file);
    }
    cout << "English dictionary create done. It has " 
         << _enDict.size() << " words." << endl;
}

void DictProducer::createCnDict()
{
    cout << "Start creating Chinese dictionary..." << endl;
    for (const auto &file : _cnFiles)
    {
        ifstream ifs;
        openCnFile(ifs, file);
    }
    cout << "Chinese dictionary create done. It has " 
         << _cnDict.size() << " words." << endl;
}

void DictProducer::createEnIndex()
{
    cout << "Start creating English index..." << endl;
    for (char c = 'a'; c <= 'z'; ++c)
    {
        string elem(1, c);
        _index[elem];
    }
    
    for (auto &it : _enDict)
    {
        _idMap[++_idCounter] = it.first;
        for (char c : it.first)
        {
            string elem(1, c);
            _index[elem].insert(_idCounter);
        }
    }
    cout << "English index create done." << endl;
}

void DictProducer::createCnIndex()
{

}

void DictProducer::store()
{
    ofstream ofs1;
    inputFile(ofs1, "en_dict.dat");

    ofstream ofs2;
    inputFile(ofs2, "en_index.dat");

    ofstream ofs3;
    inputFile(ofs3, "cn_dict.dat");

    ofstream ofs4;
    inputFile(ofs4, "cn_index.dat");
}

bool DictProducer::isStopWord(const string &word)
{
    for (auto &stopWord : _stopWords)
    {
        if (word == stopWord)
        {
            return true;
        }
    }

    return false;
}

void DictProducer::saveStopWords(const string &fileName)
{
    ifstream ifs("../data/" + fileName);
    if (!ifs.is_open())
    {
        cerr << "Open file: " << fileName << " failed!" << endl;
        return;
    }

    cout << "Open file: " << fileName << " successfully." << endl;
    cout << "Reading..." << endl;

    // 默认停用词库文件单行一个单词
    string word;
    while (getline(ifs, word))
    {
        // 清除首位空白内容
        word.erase(0, word.find_first_not_of(" \t\n\r"));
        word.erase(word.find_last_not_of(" \t\n\r") + 1);

        // 跳过空行和注释
        if (word.empty() || word[0] == '#') continue;

        _stopWords.insert(word);
    }

    cout << "Read done." << endl;
    ifs.close();
}

void DictProducer::insertEnIndex(const string &elem, size_t id)
{

}

void DictProducer::processEnLine(string &line)
{
    istringstream iss(line);
    string word;
    while (iss >> word)
    {
        string newWord = dealEnWord(word);
        if (isStopWord(newWord)) continue;
        insertToEnMap(newWord);
    }
}

void DictProducer::processCnLine(string &line)
{
    vector<string> results = wordSeg(line);
    for (auto it = results.begin(); it != results.end(); ++it)
    {
        insertToCnMap(*it);
    }
}

void DictProducer::openEnFile(ifstream &ifs, const string &fileName)
{
    ifs.open("../data/EN/" + fileName);
    if (!ifs.is_open())
    {
        cerr << "Open file: " << fileName << " failed!" << endl;
        return;
    }
    cout << "Open file: " << fileName << " successfully." << endl;
    cout << "Reading..." << endl;

    string line;
    while (getline(ifs, line))
    {
        processEnLine(line);
    }

    cout << "Read done." << endl;
}

void DictProducer::openCnFile(ifstream &ifs, const string &fileName)
{
    ifs.open("../data/CN/art/" + fileName);
    if (!ifs.is_open())
    {
        cerr << "Open file: " << fileName << " failed!" << endl;
        return;
    }
    cout << "Open file: " << fileName << " successfully." << endl;
    cout << "Reading..." << endl;

    string line;
    while (getline(ifs, line))
    {
        processCnLine(line);
    }

    cout << "Read done." << endl;
}

void DictProducer::inputFile(ofstream &ofs, const string &fileName)
{
    ofs.open("../data/" + fileName);
    if (!ofs.is_open())
    {
        cerr << "Open file: " << fileName << " failed." << endl;
        return;
    }
    cout << "Open file: " << fileName << " successfully." << endl;
    cout << "Writing..." << endl;

    if (fileName == "en_dict.dat")
    {
        for (auto it = _enDict.begin(); it != _enDict.end(); ++it)
        {
            ofs << it->first << "    " << it->second << endl;
        }
    }
    else if (fileName == "en_index.dat")
    {
        for (auto &it : _index)
        {
            ofs << "\"" << it.first << "\" : { ";
            bool first = true;
            for (auto &num : it.second)
            {
                if (first)
                {
                    ofs << num;
                    first = false;
                }
                else
                {
                    ofs << ", " << num;
                }
            }
            ofs << " }" << endl;
        }
    }
    else if (fileName == "cn_dict.dat")
    {
        for (auto it = _cnDict.begin(); it != _cnDict.end(); ++it)
        {
            ofs << it->first << "    " << it->second << endl;
        }
    }

    cout << "Write done." << endl;
    ofs.close();
}

string DictProducer::dealEnWord(const string &word)
{
    string lower;
    for (size_t idx = 0; idx != word.size(); ++idx)
    {
        if (!isalpha(word[idx]))
        {
            return string();
        }
        else
        {
            lower += std::tolower(word[idx]);
        }
    }

    return lower;
}

void DictProducer::insertToEnMap(const string &word)
{
    // 判断是否空串
    if (word == string())
    {
        return;
    }
    
    // 查看单词是否已经在unordered_map中
    auto it = _enDict.find(word);
    if (it != _enDict.end())
    {
        ++_enDict[word];    // 更新频率
    }
    else
    {
        _enDict[word] = 1;
    }
}

void DictProducer::insertToCnMap(const string &word)
{
    // 判断是否空串
    if (word == string())
    {
        return;
    }
    
    // 查看单词是否已经在unordered_map中
    auto it = _cnDict.find(word);
    if (it != _cnDict.end())
    {
        ++_cnDict[word];    // 更新频率
    }
    else
    {
        _cnDict[word] = 1;
    }

}
