#include "Configuration.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;

Configuration &Configuration::getInstance()
{
    static Configuration instance;
    return instance;
}

Configuration::Configuration()
: _configFilePath(PAGE_CONF_PATH)
{
    string page_path = PAGE_CONF_PATH;
    size_t pos = page_path.find_last_of('/');
    string name = page_path.substr(pos + 1);
    ifstream ifs(_configFilePath);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open config file \"" << name << "\" failed" << endl;
        return;
    }

    cout << "[INFO] Open config file \"" << name << "\" successfully. Reading..." << endl;

    string line;
    while (getline(ifs, line))
    {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos - 1);
        string value = line.substr(pos + 2);

        string conf_path = _configFilePath;
        size_t dir_pos = conf_path.find_last_of('/');
        string conf_dir = conf_path.substr(0, dir_pos);

        size_t last = value.find_first_not_of("../");
        size_t n = 0;
        
        while (n < last)
        {
            size_t pos = conf_dir.find_last_of('/');
            conf_dir = conf_dir.substr(0, pos);
            n += 3;
        }

        if (last != 0)
        {
            _configs[key] = conf_dir + "/" + value.substr(last);
        }
        else
        {
            _configs[key] = value;
        }
    }
    cout << "[INFO] Read done." << endl;
}

const unordered_map<string, string> &Configuration::getConfigMap() const
{
    return _configs;
}

const string &Configuration::getConfig(const string &key) const
{
    auto it = _configs.find(key);
    if (it == _configs.end())
    {
        cerr << "[ERROR] Cannot find config key: \"" << key << "\"." << endl;
    }
    else
    {
        cout << "[INFO] Find config key: \"" << key << "\"." << endl;
    }

    return it->second;
}

