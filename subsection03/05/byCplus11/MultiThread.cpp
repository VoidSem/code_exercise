/*
 * name: MultiPthread.h
 * Description: multi Thread access members(C++11)
 * author:liuxueneng@airfly
 * date:20190418
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
    totalTime = 0;
}

/* get now time us */
long long MultiThread::GetNowMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void MultiThread::PrintShow()
{
    cout << "p = " << p <<endl;
    cout << "q = " << q <<endl;
    cout << "r = " << r <<endl;
    cout << "s = " << s <<endl;
    //cout << "Caculation Take " <<  totalTime << " ms" << endl;
}

void MultiThread::RunThread()
{
    void *rst = NULL;
    long long startTime = GetNowMs();

    /* creat new thread */
    for (int i = 1; i < 5; ++i)
    {
        threads.push_back(thread(bind(ThreadHandle, i, (void *)this)));
    }

    // lambda
    for_each(threads.begin(), threads.end(), [](thread &t){ t.join();});

    //wait for thread finish
    //
    long long endTime = GetNowMs();
    totalTime = endTime - startTime;
}


void MultiThread::ThreadHandle(int threadId, void *arg)
{
    MultiThread *pThread = (MultiThread *)arg;
    int64_t count = pThread->maxNum;

    switch (threadId) {
        case 1:
            {
                while (count--) {
                    pThread->HandleThread1();
                }
                return;
            }
        case 2:
            {
                while (count--) {
                    pThread->HandleThread2();
                }
                return;
            }
        case 3:
            {
                while (count--) {
                    pThread->HandleThread3();
                }
                return;
            }
        case 4:
            {
                while (count--) {
                    pThread->HandleThread4();
                }
                return;
            }
        default:
            cerr<<"unkown threadId" << threadId <<endl;
    }
}


void MultiThread::HandleThread1(void)
{
    IncreaseP();
    IncreaseQ();
    IncreaseR();
    IncreaseS();
}

void MultiThread::HandleThread2(void)
{
    IncreaseP();
    IncreaseQ();
    IncreaseS();
}

void MultiThread::HandleThread3(void)
{
    IncreaseP();
    IncreaseR();
    IncreaseS();
}

void MultiThread::HandleThread4(void)
{
    IncreaseQ();
    IncreaseS();
}

void MultiThread::IncreaseP()
{
    ++p;
}

void MultiThread::IncreaseQ()
{
    ++q;
}

void MultiThread::IncreaseR()
{
    ++r;
}

void MultiThread::IncreaseS()
{
    ++s;
}

long long MultiThread::GetCaculateTime()
{
    return totalTime;
}

MultiThread::~MultiThread()
{
    // lambda
    //    for_each(threads.begin(), threads.end(), [](thread &t){ t.join();});
}
