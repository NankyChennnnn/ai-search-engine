#ifndef __WORDCONFIG_H__
#define __WORDCONFIG_H__

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class WordConfig
{
public:
    explicit WordConfig(const string &configPath);
    string get(const string &section, const string &key) const;
    string getPath(const string &section, const string &key) const;

private:
    string _configPath; // config file path
    string _configDir;  // config files dir
    unordered_map<string, unordered_map<string, string>> _data;
};

#endif
