/*
 * name: MultiPthread.cpp
 * Description: multi pthread access members
 * author:liuxueneng@airfly
 * date:20190418
 *
 */

#include "MultiPthread.h"
#include <ctime>
#include <sys/time.h>

using namespace std;

MultiPthread::MultiPthread(int64_t num)
{
    p = 0;
    q = 0;
    r = 0;
    s = 0;
    maxNum = num;
    totalTime = 0;

    /* init mutex */
    pthread_mutex_init(&pThreadMutexP, NULL);
    pthread_mutex_init(&pThreadMutexQ, NULL);
    pthread_mutex_init(&pThreadMutexR, NULL);
    pthread_mutex_init(&pThreadMutexS, NULL);


}

/* get now time us */
long long MultiPthread::GetNowMs()
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void MultiPthread::RunPthread()
{
    void *rst = NULL;
    long long startTime = GetNowMs();

    /* creat a new thread */
    pthread_create(&pthreadId1, NULL, HandlePthread1, (void *)this);
    pthread_create(&pthreadId2, NULL, HandlePthread2, (void *)this);
    pthread_create(&pthreadId3, NULL, HandlePthread3, (void *)this);
    pthread_create(&pthreadId4, NULL, HandlePthread4, (void *)this);

    //wait for the pthread finish
    pthread_join(pthreadId1, &rst);
    pthread_join(pthreadId2, &rst);
    pthread_join(pthreadId3, &rst);
    pthread_join(pthreadId4, &rst);
    long long endTime = GetNowMs();
    totalTime = endTime - startTime;
}

void MultiPthread::PrintShow()
{
    cout << "p = " << p <<endl;
    cout << "q = " << q <<endl;
    cout << "r = " << r <<endl;
    cout << "s = " << s <<endl;
    //cout << "Caculation Take " <<  totalTime << " ms" << endl;
}


void MultiPthread::ThreadHandle(int threadId, void *arg)
{
    MultiPthread *pThread = (MultiPthread *)arg;
    int64_t count = pThread->maxNum;

    switch (threadId) {
        case 1:
            {
                while (count--) {
                    pThread->HandlePthread1();
                }
                return;
            }
        case 2:
            {
                while (count--) {
                    pThread->HandlePthread2();
                }
                return;
            }
        case 3:
            {
                while (count--) {
                    pThread->HandlePthread3();
                }
                return;
            }
        case 4:
            {
                while (count--) {
                    pThread->HandlePthread4();
                }
                return;
            }
        default:
            cerr<<"unkown threadId" << threadId <<endl;
    }
}


void MultiPthread::HandlePthread1(void)
{
    IncreaseP();
    IncreaseQ();
    IncreaseR();
    IncreaseS();
}

void MultiPthread::HandlePthread2(void)
{
    IncreaseP();
    IncreaseQ();
    IncreaseS();
}

void MultiPthread::HandlePthread3(void)
{
    IncreaseP();
    IncreaseR();
    IncreaseS();
}

void MultiPthread::HandlePthread4(void)
{
    IncreaseQ();
    IncreaseS();
}

void MultiPthread::IncreaseP()
{
    pthread_mutex_lock(&pThreadMutexP);
    ++p;
    pthread_mutex_unlock(&pThreadMutexP);
}

void MultiPthread::IncreaseQ()
{
    pthread_mutex_lock(&pThreadMutexQ);
    ++q;
    pthread_mutex_unlock(&pThreadMutexQ);
}

void MultiPthread::IncreaseR()
{
    pthread_mutex_lock(&pThreadMutexR);
    ++r;
    pthread_mutex_unlock(&pThreadMutexR);
}

void MultiPthread::IncreaseS()
{
    pthread_mutex_lock(&pThreadMutexS);
    ++s;
    pthread_mutex_unlock(&pThreadMutexS);
}

long long MultiPthread::GetCaculateTime()
{
    return totalTime;
}

MultiPthread::~MultiPthread()
{
    pthread_mutex_destroy(&pThreadMutexP);
    pthread_mutex_destroy(&pThreadMutexQ);
    pthread_mutex_destroy(&pThreadMutexR);
    pthread_mutex_destroy(&pThreadMutexS);
}
