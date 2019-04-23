/*
 * Name:        MultiPthread.cpp
 * Description: multi pthread access members
 * Author:      liuxueneng@airfly
 * Date:        20190418
 * Modify:      20190423
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

    /* init mutex */
    pthread_mutex_init(&pThreadMutex, NULL);
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

    /* phtread id */
    pthread_t pthreadId1;
    pthread_t pthreadId2;
    pthread_t pthreadId3;
    pthread_t pthreadId4;

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

    cout <<"total time: "<< endTime - startTime<<" ms"<<endl;
}

void MultiPthread::PrintShow()
{
    cout << "p = " << p <<endl;
    cout << "q = " << q <<endl;
    cout << "r = " << r <<endl;
    cout << "s = " << s <<endl;
}




void *MultiPthread::HandlePthread1(void *argc)
{
    MultiPthread *p = (MultiPthread *)argc;
    p->DoThread1();
    return NULL;
}

void *MultiPthread::HandlePthread2(void *argc)
{
    MultiPthread *p = (MultiPthread *)argc;
    p->DoThread2();
    return NULL;
}

void *MultiPthread::HandlePthread3(void *argc)
{
    MultiPthread *p = (MultiPthread *)argc;
    p->DoThread3();
    return NULL;
}

void *MultiPthread::HandlePthread4(void *argc)
{
    MultiPthread *p = (MultiPthread *)argc;
    p->DoThread4();
    return NULL;
}

void MultiPthread::DoThread1()
{
    int64_t total = maxNum;
    while (total--> 0) {
        pthread_mutex_lock(&pThreadMutex);
        ++p;
        ++q;
        ++r;
        ++s;
        pthread_mutex_unlock(&pThreadMutex);
    }
}

void MultiPthread::DoThread2()
{
    int64_t total = maxNum;
    while (total--> 0) {
        pthread_mutex_lock(&pThreadMutex);
        ++p;
        ++q;
        ++s;
        pthread_mutex_unlock(&pThreadMutex);
    }
}

void MultiPthread::DoThread3()
{
    int64_t total = maxNum;
    while (total--> 0) {
        pthread_mutex_lock(&pThreadMutex);
        ++p;
        ++r;
        ++s;
        pthread_mutex_unlock(&pThreadMutex);
    }
}

void MultiPthread::DoThread4()
{
    int64_t total = maxNum;
    while (total--> 0) {
        pthread_mutex_lock(&pThreadMutex);
        ++q;
        ++s;
        pthread_mutex_unlock(&pThreadMutex);
    }
}


MultiPthread::~MultiPthread()
{
}
