#include "WordConfig.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;

WordConfig::WordConfig(const string &configPath)
: _configPath(configPath)
{
    size_t pos = configPath.find_last_of('/');
    _configDir = configPath.substr(0, pos);

    ifstream ifs(_configPath);
    if (!ifs.is_open())
    {
        cerr << "Open config file failed." << endl;
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
            string dir = _configDir;
            if (section == "output" && key != "output_dir")
            {
                dir = _data[section]["output_dir"];
                value = dir + value.substr(last_pos);
            }
            else
            {
                size_t n = 0;
                while (n != last_pos)
                {
                    size_t conf_pos = dir.find_last_of('/');
                    dir = dir.substr(0, conf_pos);
                    n += 3;
                }
                value = dir + "/" + value.substr(last_pos);
            }

            _data[section][key] = value;
        }
    }
}

string WordConfig::get(const string &section, const string &key) const
{
    string value;
    auto it = _data.find(section);
    if (it != _data.end())
    {
        auto data = it->second.find(key);
        if (data != it->second.end())
        {
            if (data->second.back() == '/')
            {
                cerr << "Error: Use getPath(section, key) to get directory." << endl;
                return "Error";
            }

            value = data->second;
            cout << "Info: Find [" << section << "][" << key << "]" << endl;
        }
    }
    return value;
}

string WordConfig::getPath(const string &section, const string &key) const
{
    string value;
    auto it = _data.find(section);
    if (it != _data.end())
    {
        auto data = it->second.find(key);
        if (data != it->second.end())
        {
            if (data->second.back() != '/')
            {
                cerr << "Error: Use get(section, key) to get file path." << endl;
                return "Error";
            }

            value = data->second;
            cout << "Info: Find [" << section << "][" << key << "]" << endl;
        }
    }
    return value;
}

