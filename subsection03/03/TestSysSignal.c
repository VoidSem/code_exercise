/*
 * Name:        TestSysSignal.c
 * Descriptor:  system signal test
 * Author:      liuxueneng@iairfly
 * Date:        20190417
 * Modify:      20190423
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

void SignalHandler(int signo)
{
    switch (signo) {
        case SIGQUIT:
        case SIGTERM:
            {
                printf("Bye\n");
                exit(0);
            }
        case SIGALRM:
            {
                printf("Hello\n");
                break;
            }
    }
}

int main(void)
{
    if (signal(SIGQUIT, SignalHandler) == SIG_ERR) {
        printf("\ncan't catch SIGQUIT\n");
        return -1;
    }

    if (signal(SIGTERM, SignalHandler) == SIG_ERR) {
        printf("\ncan't catch SIGQUIT\n");
        return -1;
    }

    if (signal(SIGALRM, SignalHandler) == SIG_ERR) {
        printf("\ncan't catch SIGALRM\n");
        return -1;
    }

    struct itimerval itv;
    struct itimerval oldTv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    /*init timer*/
    setitimer(ITIMER_REAL, &itv, &oldTv);

    for ( ; ; ) {
        //this will return when get catch a signal
        pause();
    }

    return 0;
}
