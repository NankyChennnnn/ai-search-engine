#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
#include <unordered_map>
#include <string>

using std::cerr;
using std::endl;
using std::unordered_map;
using std::string;

class Configuration
{
public:
    static Configuration &getInstance();
    const unordered_map<string, string> &getConfigMap() const;
    const string &getConfig(const string &key) const;

private:
    Configuration();
    ~Configuration() = default;
    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;

private:
    string _configFilePath;
    unordered_map<string, string> _configs;
};

#endif
