#include "JiebaConfig.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;

JiebaConfig::JiebaConfig(const string &configPath)
: _configPath(configPath)
{
    size_t pos = configPath.find_last_of('/');
    _configDir = configPath.substr(0, pos);

    ifstream ifs(_configPath);
    if (!ifs.is_open())
    {
        cerr << "[ERROR] Open config file failed." << endl;
    }

    string line;
    string section;
    string key, value;
    while (getline(ifs, line))
    {
        if (line.empty()) continue;

        if (line.front() == '[' && line.back() == ']')
        {
            section = line.substr(1, line.size() - 2);
        }

        size_t pos = line.find('=');
        if (pos != string::npos)
        {
            key = line.substr(0, pos - 1);
            value = line.substr(pos + 2);

            size_t last_pos = value.find_first_not_of("../");
            size_t n = 0;
            string dir = _configDir;
            while (n != last_pos)
            {
                size_t conf_pos = dir.find_last_of('/');
                dir = dir.substr(0, conf_pos);
                n += 3;
            }

            value = dir + "/" + value.substr(last_pos);
            _data[section][key] = value;
        }
    }
}

const string& JiebaConfig::get(const string &section, const string &key) const
{
    auto it = _data.find(section);
    if (it != _data.end())
    {
        auto data = it->second.find(key);
        if (data != it->second.end())
        {
            cout << "[INFO] Find [" << section << "][" << key << "]" << endl;
            return data->second;
        }
    }

    throw std::runtime_error("[ERROR] Cannot find [" + section + "][" + key + "]");
}
