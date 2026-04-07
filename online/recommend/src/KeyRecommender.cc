#include "KeyRecommender.h"
#include "CandidateResult.h"
#include "Dictionary.h"
#include <algorithm>

KeyRecommender::KeyRecommender(const string &word)
: _sought(word)
{
}

KeyRecommender::~KeyRecommender()
{

}

vector<CandidateResult> KeyRecommender::doQuery()
{
    vector<CandidateResult> crs;
    Dictionary &dict = Dictionary::getInstance();
    vector<pair<string, int>> results = dict.doQuery(_sought);
    for (auto rslt : results)
    {
        CandidateResult cr;
        cr._word = rslt.first;
        cr._freq = rslt.second;
        cr._dist = distance(_sought, rslt.first);
        crs.push_back(cr);
    }
    
    std::sort(crs.begin(), crs.end(), 
              [](const CandidateResult &lhs, const CandidateResult &rhs) {
                  if (lhs._dist != rhs._dist) return lhs._dist < rhs._dist;
                  if (lhs._freq != rhs._freq) return lhs._freq > rhs._freq;
                  return lhs._word < rhs._word;
              });

    vector<CandidateResult> topCrs;
    int topk = dict.getTopk();
    for (int i = 0; i < topk; ++i)
    {
        topCrs.push_back(crs[i]);
    }
    return topCrs;
}

int KeyRecommender::distance(const string &word, const string &cand)
{
    // Levenshtein 编辑距离：用于衡量两个字符串之间差异的度量标准
    // 包含三种操作：插入、删除、替换字符
    // 假设两字符串 a 和 b，长度为|a|和|b|
    // lev(i, j)表示a的前i个字符，与b的前j个字符之间的距离
    // 递推公式：
    // if min(i, j) = 0, return max(i, j)
    // else min{ {lev(i-1, j)+1}, {lev(i, j-1)+1}, {lev(i-1, j-1)+(ai != bj)} }

    size_t m = word.size();
    size_t n = cand.size();

    /* // 时间复杂度：O(3 * (m + n)) */
    /* if (m == 0 || n == 0) */ 
    /* { */
    /*     return std::max(m, n); */
    /* } */
    /* else */
    /* { */
    /*     int min1 = distance(word.substr(1), cand) + 1; */
    /*     int min2 = distance(word, cand.substr(1)) + 1; */
    /*     int min3 = distance(word.substr(1), cand.substr(1)) */ 
    /*         + ((word[0] == cand[0]) ? 0 : 1); */
    /*     return std::min({min1, min2, min3}); */
    /* } */

    // 时间复杂度：O(m * n)
    // 生成 (m + 1) x (n + 1) 的二维矩阵
    // mat[i][j] 表示把 word 的前 i 个字符变成 cand 的前 j 个字符，最少要几步
    vector<vector<int>> mat(m + 1, vector<int>(n + 1, 0));

    // 矩阵初始化
    // word为空，转换为cand的前j个字符，需要插入j次，距离为j
    // 假设 word = ""，cand = "hello"
    // ""->"h": (0, 1) = 1; ""->"he": (0, 2) = 2; ...
    for (size_t j = 0; j <= n; ++j)
    {
        mat[0][j] = static_cast<int>(j);
    }
    // cand为空，word的前i个字符转为空，需要删除i次，距离为i
    // 假设 word = "helo"，cand = ""
    // "h"->"": (1, 0) = 1; "he"->"": (2, 0) = 2; ...
    for (size_t i = 0; i <= m; ++i)
    {
        mat[i][0] = static_cast<int>(i);
    }

    // 计算 Levenshtein 编辑距离
    // 假设 word = "helo", cand = "hello"
    for (size_t i = 1; i <= m; ++i)
    {
        for (size_t j = 1; j <= n; ++j)
        {
            // mat 主对角线判断，字符相等，不用替换
            // i = j 时的字符判断
            // word[0] = 'h' == cand[0] = 'h'
            if (word[i - 1] == cand[j - 1])
            {
                mat[i][j] = mat[i - 1][j - 1];
            }
            else
            {
                // i != j 时的字符判断
                // 如 (i, j) = (1, 2)，等价于将 "h" 替换成 "he"，最少需要几步
                // 此时
                // del = mat[0][2] + 1 = 3;
                // ins = mat[1][1] + 1 = 1;
                // rep = mat[0][1] + 1 = 2;
                // 所以取最小值ins，即要插入一个字符

                // 1. 如果删除 word 当前字符
                int del = mat[i - 1][j] + 1;

                // 2. 如果向 word 中插入 cand 当前字符
                int ins = mat[i][j - 1] + 1;

                // 3. 如果把 word 当前字符替换成 cand 当前字符
                int rep = mat[i - 1][j - 1] + 1;

                mat[i][j] = std::min({del, ins, rep});
            }
        }
    }

    // distance = word 全部字符 -> cand 全部字符的最少步数
    return mat[m][n];
}

