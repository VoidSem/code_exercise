/*
 * name: MultiPthread.h
 * Description: multi pthread access members
 * author:liuxueneng@airfly
 * date:20190418
 *
 */

#ifndef MULTI_PTHREAD_H
#define MULTI_PTHREAD_H

#include <iostream>
#include <pthread.h>
#include <stdint.h>

class MultiPthread
{
    public:
        MultiPthread(int64_t num);
        void RunPthread();
        void PrintShow();

        /*increase val*/
        void IncreaseP();
        void IncreaseQ();
        void IncreaseR();
        void IncreaseS();

        long long GetCaculateTime();

        static long long GetNowMs();
        /* pthread handlers */
        static void* HandlePthread1(void *arg);
        static void* HandlePthread2(void *arg);
        static void* HandlePthread3(void *arg);
        static void* HandlePthread4(void *arg);


        virtual ~MultiPthread();

    protected:

    private:
        /* data members */
        int64_t p;
        int64_t q;
        int64_t r;
        int64_t s;

        /**/
        int64_t maxNum;
        /* caculate time */
        long long totalTime;

        /* phtread id */
        pthread_t pthreadId1;
        pthread_t pthreadId2;
        pthread_t pthreadId3;
        pthread_t pthreadId4;

        /* mutexs */
        pthread_mutex_t pThreadMutexP;
        pthread_mutex_t pThreadMutexQ;
        pthread_mutex_t pThreadMutexR;
        pthread_mutex_t pThreadMutexS;

};

#endif // MULTI_PTHREAD_H
