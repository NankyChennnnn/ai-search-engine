#include "KeyRecommender.h"
#include "Dictionary.h"

using std::cout;
using std::endl;

KeyRecommender::KeyRecommender(const string &word)
{
    Dictionary &dict = Dictionary::getInstance();
    vector<string> results = dict.doQuery(word);
    cout << "[INFO] Query result: ";
    for (auto rslt : results)
    {
        cout << "\"" << rslt << "\" ";
    }
    cout << endl;
}

KeyRecommender::~KeyRecommender()
{

}
