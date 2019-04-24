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
#include <time.h>

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
    int sum = 4;

    /* phtread id */
    pthreadArg_t pArg[sum] = {
        {0x1 | 0x1 << 1 | 0x1 << 2 | 0x1 << 3},
        {0x1 | 0x1 << 1 | 0x1 << 3},
        {0x1 | 0x1 << 2 | 0x1 << 3},
        {0x1 << 1 | 0x1 << 3}};

    long long startTime = GetNowMs();
    /* creat a new thread */
    pthread_t pthreadId[sum];
    for(int i = 0; i < sum; ++i) {
        pArg[i].p = (void *)this;
        pthread_create(&pthreadId[i], NULL, HandlePthread, (void *)&pArg[i]);
    }

    //wait for the pthread finish
    void *rst = NULL;
    for(int i = 0; i < sum; ++i) {
        pthread_join(pthreadId[i], &rst);
    }
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

void *MultiPthread::HandlePthread(void *argc)
{
    pthreadArg_t *tmpArg = (pthreadArg_t *)argc;
    MultiPthread *p = (MultiPthread *)tmpArg->p;
    /*pthread do*/
    p->DoThread(tmpArg->mode);

    return NULL;
}

void MultiPthread::DoThread(unsigned char mode)
{
    int64_t total = maxNum;
    while (total-- > 0) {
        pthread_mutex_lock(&pThreadMutex);
        if (mode & 0x1) {
            ++p;
        }
        if (mode & (0x1 << 1) ) {
            ++q;
        }
        if (mode & (0x1 << 2) ) {
            ++r;
        }
        if (mode & (0x1 << 3) ) {
            ++s;
        }
        pthread_mutex_unlock(&pThreadMutex);
    }
}

MultiPthread::~MultiPthread()
{
}
