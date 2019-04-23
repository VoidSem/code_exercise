/*
 * Name:        TestThreadDemo.cpp
 * Description: multi Thread test (C++11)
 * Author:      liuxueneng@airfly
 * Date:        20190418
 * Modify:      20190423
 */

#include <iostream>
#include "MultiThread.h"

using namespace std;

#define      MAX_COUNT_NUM   (1e+7)

int main()
{
    MultiThread *testThread = new MultiThread(MAX_COUNT_NUM);

    testThread->RunThread();

    testThread->PrintShow();

    //free
    delete testThread;

    return 0;
}
