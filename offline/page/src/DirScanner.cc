#include "DirScanner.h"
#include <iostream>
#include <dirent.h>
#include <stdexcept>
#include <string.h>

using std::cout;
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
        throw std::runtime_error("[ERROR] This " + dir + " is null.");
    }

    cout << "[INFO] Find dir: \"" << dir << "\". Reading..." << endl;

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

