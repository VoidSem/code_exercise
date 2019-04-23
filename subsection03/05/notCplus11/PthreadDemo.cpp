/*
 * Name:        TestPthreadDemo.cpp
 * Description: multi pthread test
 * Author:      liuxueneng@iairfly
 * Date:        20190418
 * Modify:      20190423
 */

#include <iostream>
#include "MultiPthread.h"

using namespace std;

#define      MAX_COUNT_NUM   (1e+7)

int main()
{
    MultiPthread *manager = new MultiPthread(MAX_COUNT_NUM);

    manager->RunPthread();

    manager->PrintShow();

    //free
    delete manager;

    return 0;
}
