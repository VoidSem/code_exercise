/*
 * name:        TestPthreadDemo.cpp
 * Description: multi pthread test
 * author:      liuxueneng@airfly
 * date:        20190418
 *
 */

#include <iostream>
#include "MultiPthread.h"

using namespace std;

#define      MAX_COUNT_NUM   (1e+9)

int main()
{
    MultiPthread *manager = new MultiPthread(MAX_COUNT_NUM);

    manager->PrintShow();

    cout << "start increase " << MAX_COUNT_NUM <<" times" <<endl;
    manager->RunPthread();

    cout << "increase " << MAX_COUNT_NUM <<
        " times take " << manager->GetCaculateTime() << " ms"<<endl;
    manager->PrintShow();

    //free
    delete manager;
    return 0;
}
