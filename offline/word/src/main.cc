#include "DictProducer.h"

int main()
{
    cout << "=============== Dictionary Producer Init ===============" << endl;
    DictProducer dp;
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

