#include "DictProducer.h"
#include "WordSegmentation.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

using std::cout;
using std::endl;
using std::cerr;
using std::istringstream;

DictProducer::DictProducer(WordConfig wordConf)
: _enIdCounter(0)
, _cnIdCounter(0)
, _wordConf(wordConf)
{
    saveStopWords(_wordConf.get("data", "stop_words_eng"));
    saveStopWords(_wordConf.get("data", "stop_words_zh"));
}

DictProducer::~DictProducer()
{

}

void DictProducer::createEnDict()
{
    cout << "[INFO] Start creating English dictionary..." << endl;
    _enFiles.push_back(_wordConf.get("data", "english"));
    _enFiles.push_back(_wordConf.get("data", "bible"));
    
    for (const auto &file : _enFiles)
    {
        // 防止文件类型不同
        ifstream ifs;
        openEnFile(ifs, file);
    }
    cout << "[INFO] English dictionary create done. It has " 
         << _enDict.size() << " words." << endl;
}

void DictProducer::createCnDict()
{
    cout << "[INFO] Start creating Chinese dictionary..." << endl;
    string dirname = _wordConf.getPath("data", "chinese_dir");
    DIR* dir;
    struct dirent* direntptr;
    dir = opendir(dirname.c_str());
    if (!dir)
    {
        cerr << "[ERROR] This " << dirname.c_str() << " is null." << endl;
        return;
    }

    while((direntptr = readdir(dir)) != nullptr)
    {
        // 去掉当前目录和上一级目录，避免死循环
        if (strncmp(direntptr->d_name, ".", 1) != 0 &&
            strncmp(direntptr->d_name, "..", 2) != 0)
        {
            _cnFiles.push_back(dirname + direntptr->d_name);
        }
    }

    for (const auto &file : _cnFiles)
    {
        ifstream ifs;
        openCnFile(ifs, file);
    }
    cout << "[INFO] Chinese dictionary create done. It has " 
         << _cnDict.size() << " words." << endl;
}

void DictProducer::createEnIndex()
{
    cout << "[INFO] Start creating English index..." << endl;
    for (char c = 'a'; c <= 'z'; ++c)
    {
        string elem(1, c);
        _enIndex[elem];
    }
    
    for (auto &it : _enDict)
    {
        _enIdMap[++_enIdCounter] = it.first;
        for (char c : it.first)
        {
            string elem(1, c);
            _enIndex[elem].insert(_enIdCounter);
        }
    }
    cout << "[INFO] English index create done." << endl;
}

void DictProducer::createCnIndex()
{
    cout << "[INFO] Start creating Chinese index..." << endl;
    for (auto &it : _cnDict)
    {
        // 分配id
        _cnIdMap[++_cnIdCounter] = it.first;

        // 拆分汉字，插入id
        vector<string> words = _wordSeg.DecodeRunesInString(it.first);
        for (auto it = words.begin(); it != words.end(); ++it)
        {
            _cnIndex[*it].insert(_cnIdCounter);
        }
    }
    cout << "[INFO] Chinese index create done." << endl;
}

void DictProducer::store()
{
    ofstream ofs1;
    inputFile(ofs1, _wordConf.get("output", "en_dict"));

    ofstream ofs2;
    inputFile(ofs2, _wordConf.get("output", "en_index"));

    ofstream ofs3;
    inputFile(ofs3, _wordConf.get("output", "cn_dict"));

    ofstream ofs4;
    inputFile(ofs4, _wordConf.get("output", "cn_index"));
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
    ifstream ifs(fileName);
    size_t pos = fileName.find_last_of('/');
    string name = fileName.substr(pos + 1);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open file \"" << name << "\" failed!" << endl;
        return;
    }

    cout << "[INFO] Open file \"" << name << "\" successfully. Reading..." << endl;

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

    cout << "[INFO] Read done." << endl;
    ifs.close();
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
    vector<string> results = _wordSeg(line);
    for (auto it = results.begin(); it != results.end(); ++it)
    {
        if (isStopWord(*it)) continue;
        insertToCnMap(*it);
    }
}

void DictProducer::openEnFile(ifstream &ifs, const string &fileName)
{
    ifs.open(fileName);
    size_t pos = fileName.find_last_of('/');
    string name = fileName.substr(pos + 1);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open file \"" << name << "\" failed!" << endl;
        return;
    }

    cout << "[INFO] Open file \"" << name << "\" successfully. Reading..." << endl;

    string line;
    while (getline(ifs, line))
    {
        processEnLine(line);
    }

    cout << "[INFO] Read done." << endl;
}

void DictProducer::openCnFile(ifstream &ifs, const string &fileName)
{
    ifs.open(fileName);
    size_t pos = fileName.find_last_of('/');
    string name = fileName.substr(pos + 1);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open file \"" << name << "\" failed!" << endl;
        return;
    }

    cout << "[INFO] Open file \"" << name << "\" successfully. Reading..." << endl;

    string line;
    while (getline(ifs, line))
    {
        processCnLine(line);
    }

    cout << "[INFO] Read done." << endl;
}

void DictProducer::inputFile(ofstream &ofs, const string &fileName)
{
    string output_dir = _wordConf.getPath("output", "output_dir");
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

    ofs.open(fileName);
    size_t pos = fileName.find_last_of('/');
    string name = fileName.substr(pos + 1);
    if (!ofs.is_open())
    {
        cerr << "[ERROR] Open file \"" << name << "\" failed!" << endl;
        return;
    }

    cout << "[INFO] Open file \"" << name << "\" successfully. Writing..." << endl;

    if (name == "en_dict.dat")
    {
        for (auto it = _enDict.begin(); it != _enDict.end(); ++it)
        {
            ofs << it->first << "    " << it->second << endl;
        }
    }
    else if (name == "en_index.dat")
    {
        for (auto &it : _enIndex)
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
    else if (name == "cn_dict.dat")
    {
        for (auto it = _cnDict.begin(); it != _cnDict.end(); ++it)
        {
            ofs << it->first << "    " << it->second << endl;
        }
    }
    else if (name == "cn_index.dat")
    {
        for (auto &it : _cnIndex)
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

    cout << "[INFO] Write done." << endl;
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
