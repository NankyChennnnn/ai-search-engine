#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

class DirScanner
{
public:
    DirScanner();
    ~DirScanner();

    vector<string>& getFiles();
    void traverse(string dir);

private:
    vector<string> _files;
};

#endif
