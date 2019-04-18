/*
 * name: MultiThread.h
 * Description: multi Thread access members(C++11)
 * author:liuxueneng@airfly
 * date:20190418
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

        long long GetCaculateTime();

        static long long GetNowMs();
        static void ThreadHandle(int threadId, void *arg);

        /* pthread handlers */
        void HandleThread1(void);
        void HandleThread2(void);
        void HandleThread3(void);
        void HandleThread4(void);


        virtual ~MultiThread();

    protected:

    private:
        /* atomic member*/
        std::atomic<int64_t> p;
        std::atomic<int64_t> q;
        std::atomic<int64_t> r;
        std::atomic<int64_t> s;

        /*increase val*/
        void IncreaseP();
        void IncreaseQ();
        void IncreaseR();
        void IncreaseS();

        /*counter*/
        int64_t maxNum;

        /* caculate time */
        long long totalTime;

};

#endif // MULTI_THREAD_H
