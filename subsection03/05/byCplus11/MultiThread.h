/*
 * Name:        MultiThread.h
 * Description: multiThread hearder
 * Author:      liuxueneng@iairfly
 * Date:        20190418
 * Modify:      20190423
 */

#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include <stdint.h>


class MultiThread
{
    public:
        MultiThread(int64_t num);
        void RunThread();
        void PrintShow();

        std::vector<std::thread> threads;

        static long long GetNowMs();
        static void ThreadHandle(unsigned char mode, void *arg);

        /* pthread handlers */
        void HandleIncrease(unsigned char mode);

        virtual ~MultiThread();

    protected:

    private:
        /* atomic member*/
        std::atomic<int64_t> p;
        std::atomic<int64_t> q;
        std::atomic<int64_t> r;
        std::atomic<int64_t> s;

        /*counter*/
        int64_t maxNum;
};

#endif // MULTI_THREAD_H
