/*
 * Name:        MultiPthread.h
 * Description: multi pthread access members
 * Author:      liuxueneng@iairfly
 * Date:        20190418
 * Modify:      20190423
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
        void DoThread1();
        void DoThread2();
        void DoThread3();
        void DoThread4();

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

        int64_t maxNum;
        /* mutexs */
        pthread_mutex_t pThreadMutex;

};

#endif // MULTI_PTHREAD_H
