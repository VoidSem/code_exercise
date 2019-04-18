/*
 * name:        TestThreadDemo.cpp
 * Description: multi Thread test (C++11)
 * author:      liuxueneng@airfly
 * date:        20190418
 *
 */

#include <iostream>
#include "MultiThread.h"

using namespace std;

#define      MAX_COUNT_NUM   (1e+9)

int main()
{
    MultiThread *testThread = new MultiThread(MAX_COUNT_NUM);

    testThread->PrintShow();

    cout << "start increase " << MAX_COUNT_NUM <<" times" <<endl;
    testThread->RunThread();

    cout << "increase " << MAX_COUNT_NUM <<
        " times take " << testThread->GetCaculateTime() << " ms"<<endl;
    testThread->PrintShow();

    //free
    delete testThread;
    return 0;
}
