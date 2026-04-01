#ifndef __JIEBACONFIG_H__
#define __JIEBACONFIG_H__

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class JiebaConfig
{
public:
    explicit JiebaConfig(const string &configPath);
    const string &get(const string &section, const string &key) const;

private:
    string _configPath; // config file path
    unordered_map<string, unordered_map<string, string>> _data;
};

#endif
