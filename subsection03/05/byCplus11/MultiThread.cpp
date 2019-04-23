/*
 * Name:        MultiPthread.cpp
 * Description: multi Thread access members(C++11)
 * Author:      liuxueneng@iairfly
 * Date:        20190418
 * Modify:      20190423
 */

#include "MultiThread.h"
#include <ctime>
#include <sys/time.h>

using namespace std;

MultiThread::MultiThread(int64_t num)
{
    p = 0;
    q = 0;
    r = 0;
    s = 0;
    maxNum = num;
}

/* get now time us */
long long MultiThread::GetNowMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

/*out put*/
void MultiThread::PrintShow()
{
    cout << "p = " << p <<endl;
    cout << "q = " << q <<endl;
    cout << "r = " << r <<endl;
    cout << "s = " << s <<endl;
}

void MultiThread::HandleIncrease(unsigned char mode)
{
    int64_t total = maxNum;

    while (total--) {
        if (mode & 0x1) {
            ++p;
        }
        if (mode & (0x1 << 1)) {
            ++q;
        }
        if (mode & (0x1 << 2)) {
            ++r;
        }
        if (mode & (0x1 << 3)) {
            ++s;
        }
    }
}

/*create thread and run*/
void MultiThread::RunThread()
{
    int tNum = 4;
    unsigned char mode[tNum] = {
        0x1 | 0x1 << 1 | 0x1 << 2 | 0x1 << 3,
        0x1 | 0x1 << 1 | 0x1 << 3,
        0x1 | 0x1 << 2 | 0x1 << 3,
        0x1 << 1 | 0x1 << 3};

    long long startTime = GetNowMs();
    /* creat new thread */
    for (int i = 0; i < tNum; ++i)
    {
        threads.push_back(thread(bind(ThreadHandle, mode[i], (void *)this)));
    }

    //wait for thread finish
    for_each(threads.begin(), threads.end(), [](thread &t){ t.join();});

    long long endTime = GetNowMs();
    cout <<"totalTime "<<endTime - startTime<<" ms"<<endl;
}

void MultiThread::ThreadHandle(unsigned char mode, void *arg)
{
    MultiThread *p = (MultiThread *)arg;
    p->HandleIncrease(mode);
}


MultiThread::~MultiThread()
{
}
