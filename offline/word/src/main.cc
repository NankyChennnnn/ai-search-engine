#include "WordConfig.h"
#include "DictProducer.h"

int main()
{
    WordConfig wc(WORD_CONF_PATH);
    string value = wc.get("data", "chinese_dir");
    value = wc.getPath("data", "chinese_dir");
    value = wc.get("data", "english");

    cout << "=============== Dictionary Producer Init ===============" << endl;
    DictProducer dp(wc);
    cout << endl << "=============== Create English Dictionary ===============" << endl;
    dp.createEnDict();
    cout << endl << "=============== Create English Index ===============" << endl;
    dp.createEnIndex();
    cout << endl << "=============== Create Chinese Dictionary ===============" << endl;
    dp.createCnDict();
    cout << endl << "=============== Store .dat Files ===============" << endl;
    dp.store();
    return 0;
}

