#include "DirScanner.h"
#include <iostream>
#include <dirent.h>
#include <string.h>

using std::cout;
using std::cerr;
using std::endl;

DirScanner::DirScanner()
{

}

DirScanner::~DirScanner()
{

}

vector<string>& DirScanner::getFiles()
{
    return _files;
}

void DirScanner::traverse(string dir)
{
    DIR* pDir;
    struct dirent* direntptr;
    pDir = opendir(dir.c_str());
    if (!pDir)
    {
        cerr << "[ERROR] This " << dir.c_str() << " is null." << endl;
        return;
    }

    cout << "[INFO] Find dir: \"" << dir.c_str() << "\". Reading..." << endl;

    while((direntptr = readdir(pDir)) != nullptr)
    {
        // 去掉当前目录和上一级目录，避免死循环
        if (strncmp(direntptr->d_name, ".", 1) != 0 &&
            strncmp(direntptr->d_name, "..", 2) != 0)
        {
            _files.push_back(dir + direntptr->d_name);
        }
    }
    cout << "[INFO] Read done." << endl;
}

