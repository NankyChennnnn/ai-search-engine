#include "WordConfig.h"
#include "DictProducer.h"

int main()
{
    WordConfig wc(WORD_CONF_PATH);

    cout << "=============== Dictionary Producer Init ===============" << endl;
    DictProducer dp(wc);
    
    cout << endl << "=============== Create English Dictionary ===============" << endl;
    dp.createEnDict();
    cout << endl << "=============== Create English Index ===============" << endl;
    dp.createEnIndex();
    cout << endl << "=============== Create Chinese Dictionary ===============" << endl;
    dp.createCnDict();
    cout << endl << "=============== Create Chinese Index ===============" << endl;
    dp.createCnIndex();
    cout << endl << "=============== Store .dat Files ===============" << endl;
    dp.store();
    return 0;
}

