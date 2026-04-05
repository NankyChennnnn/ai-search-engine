#include "PageLibPreprocessor.h"
#include "simhash/Simhasher.hpp"

PageLibPreprocessor::PageLibPreprocessor()
{

}

PageLibPreprocessor::~PageLibPreprocessor()
{

}

void PageLibPreprocessor::cutRedundantPage(vector<RssItem> &rss, 
                                           const simhash::Simhasher &simhasher)
{
    // 计算每个rss内容的simhash值
    vector<pair<int, uint64_t>> Sim;
    size_t topN = 20;
    for(size_t i = 0; i < rss.size(); ++i)
    {
        uint64_t u64 = 0;
		// make 函数的作用：根据前 topN 个权重计算降维后的 Simhash 值
		// 计算后的 Simhash 值为 u64，将其存放在 vector 中
        // topN 保留权重最高的 topN 个词
        simhasher.make(rss[i].description, topN, u64);
        Sim.push_back(std::make_pair(i, u64));
    }

    // 获取待去重内容
    set<int> tmp;
    for (size_t it = 0; it < Sim.size(); ++it)
    {
        int id1 = Sim[it].first; // 拿到 simhash 对应的 id
        int sz1 = rss[id1].description.size(); // 拿到 it1 对应 id 的内容，得到 size
        for (size_t it2 = (it + 1); it2 < Sim.size(); ++it2)
        {
            // it 与后方所有 simhash 通过海明距离进行比较
            // 海明距离计算
            // 二进制串A和二进制串B的海明距离 = A xor B后二进制中1的个数
            // isEqual 第三个参数为海明距离，默认阈值为3，可传参更改
            // 如果两个 simhash 相同
            if (simhash::Simhasher::isEqual(Sim[it].second, Sim[it2].second))
            {
                int id2 = Sim[it2].first; // 拿到待比较的 simhash 对应的 id
                int sz2 = rss[id2].description.size(); // 拿到 it2 对应 id 的内容，得到 size
                // 把内容更少的存放到set容器中，方便后续删除
                if (sz1 >= sz2)
                {
                    // 如果 id2 被删掉，继续遍历找下一个相同simhash
                    tmp.insert(id2);
                }
                else
                {
                    tmp.insert(id1);
                    // 如果 id1 就被删掉了，可以 break 找下一个了
                    break;
                }
            }
        }
    }

    // 遍历删除
    for (auto rtmp = tmp.rbegin(); rtmp != tmp.rend(); ++rtmp)
    {
        int id = *rtmp;
        if (id >= 0 && static_cast<size_t>(id) < rss.size())
        {
            rss.erase(rss.begin() + id);
        }
    }
}

void PageLibPreprocessor::
buildInvertIndexMap(vector<RssItem> &rss,
                    unordered_map<string, set<pair<int , double>>> &invertIndex,
                    const cppjieba::Jieba &jieba)
{
    size_t DOCNUM = rss.size();
    unordered_map<string, unordered_map<int, double>> tf; // term frequncy 词频
                                                          // <word, <docid, TF>>
    // cut word just use description
    int docid = 1;
    for (auto it = rss.begin(); it != rss.end(); ++it, ++docid)
    {
        string description = it->description;
        vector<string> results;
        jieba.CutAll(description, results);
        for (auto &word : results)
        {
            if (tf[word][docid])
            {
                ++tf[word][docid];                
            }
            else
            {
                tf[word][docid] = 1;
            }
        }
    }

    // TF-IDF 算法
    // TF 词频，IDF 逆文档频率 Inverse Document Frequency
    // 词出现的文章数目越少，就越稀有，区分度越高，IDF 越高
    unordered_map<int, double> wsos; // Weighted sum of squares 权重平方和
    for (auto it = tf.begin(); it != tf.end(); ++it)
    {
        string word = it->first;
        size_t DF = it->second.size(); // word in DF docs
        for (auto num = it->second.begin(); num != it->second.end(); ++num)
        {
            int docid = num->first;
            int TF = num->second;
            
            // IDF = log2(总文档数 / (包含该词的文档数 + 1))
            // DF 越大，分母越大，IDF 越小，稀有度越低
            // +1 是为了防止分母特殊情况，并且让公式更平滑
            double IDF = log(1.0 * DOCNUM / (DF + 1)) / log(2);
            
            // final weight
            // 当前文章出现的多（TF大），整体网页库中又出现的少（IDF大），权重就会大
            double w = TF * IDF;
            num->second = w; // 用权重覆盖词频
            
            // 归一化处理：把每篇文档的向量缩放到一个统一尺度，即单位向量
            // 如果文章很长，TF就会更大，那w就会更大
            // 这样对短文章不公平，所以要进行归一化处理
            // 整体思路：
            // 整个网页库可以看作一个向量空间
            // 词频统计容器中的每个词都对应向量空间中的一个维度
            // 每篇文章对应了一个向量A
            // 一篇文章中的各个词对应的权重，组成了这个向量空间中的坐标值
            // 也就是 A = (w_a, w_b, w_c, ...)
            // 然后根据这个向量取模长，再进行归一化处理成单位向量
            // 与此同时，到时候查询词也会进行归一化处理，放到这个向量空间
            // 与所以文章做比较，两向量的夹角越小，方向余弦也就越大
            // 查询词与文章的匹配程度也就越高，相关性越高
            wsos[docid] += w * w; // 每篇文章的权重平方和
        }
    }

    for (auto it = tf.begin(); it != tf.end(); ++it)
    {
        string word = it->first;
        for (auto num = it->second.begin(); num != it->second.end(); ++num)
        {
            int docid = num->first;
            double modulus = sqrt(wsos[docid]);
            double w = num->second;
            double final_w = w / modulus;
            // <word, <docid, final_w>>
            invertIndex[word].insert(std::make_pair(docid, final_w));
        }
    }
}

void PageLibPreprocessor::storeOnDisk()
{

}
