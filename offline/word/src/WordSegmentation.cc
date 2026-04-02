#include "WordSegmentation.h"
#include "cppjieba/Unicode.hpp"

JiebaConfig _jiebaConf(JIEBA_CONF_PATH);

WordSegmentation::WordSegmentation()
: _jieba(_jiebaConf.get("jieba", "dict").c_str(),       // 最大概率法
                                                        // (MPSegment: Max Probability)
                                                        // 分词所使用的词典路径
         
         _jiebaConf.get("jieba", "hmm").c_str(),        // 隐式马尔科夫模型
                                                        // (HMMSegment: Hidden Markov Model)
                                                        // 分词所使用的词典路径
         
         _jiebaConf.get("jieba", "user").c_str(),       // 用户自定义词典路径
         
         _jiebaConf.get("jieba", "idf").c_str(),        // IDF路径
         
         _jiebaConf.get("jieba", "stop_word").c_str())  // 停用词路径
{
    cout << "[INFO] Jieba inited." << endl;
}

vector<string> WordSegmentation::operator()(const string str)
{
    // 返回分词结果
    vector<string> words;
    _jieba.CutAll(str, words);
    return words;
}

vector<string> WordSegmentation::DecodeRunesInString(const string &str)
{
    vector<string> words;
    cppjieba::RuneStrArray runes;
    if (!cppjieba::DecodeRunesInString(str, runes))
    {
        return words;
    }

    words.reserve(runes.size());
    for (const auto &r : runes)
    {
        words.push_back(str.substr(r.offset, r.len));
    }

    return words;
}
