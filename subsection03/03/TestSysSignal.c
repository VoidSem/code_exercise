/*
 * file:        TestSysSignal.c
 * func:        system signal handler
 * author:      liuxueneng@airfly
 * date:        20190417
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


void sig_handler(int signo)
{
    if (signo == SIGQUIT || signo == SIGTERM) {
        printf("Bye\n");
        exit(0);
    }
}

int main(void)
{
    if (signal(SIGQUIT, sig_handler) == SIG_ERR) {
        printf("\ncan't catch SIGINT\n");
        return -1;
    }

    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        printf("\ncan't catch SIGINT\n");
        return -1;
    }

    while (1) {
        printf("Hell0\n");
        sleep(1);
    }

    return 0;
}
