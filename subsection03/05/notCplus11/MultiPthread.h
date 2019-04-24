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

typedef struct pthreadArg_s {
    unsigned char       mode;
    void                *p;
}pthreadArg_t;

class MultiPthread
{
    public:
        MultiPthread(int64_t num);

        void RunPthread();

        void PrintShow();

        /*increase val*/
        void DoThread(unsigned char mode);

        static long long GetNowMs();

        /* pthread handlers */
        static void* HandlePthread(void *arg);

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
