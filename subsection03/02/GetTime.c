/*
 * Name:        TestGetTime.c
 * Description: get time test demo
 * Author:      liuxueneng@airfly
 * Date:        20190417
 * Modify:      20190423
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    time_t timeNow;
    time(&timeNow);

    printf("Unix  time:[ %ld ]\n", timeNow);

    struct tm *curTm = localtime(&timeNow);

    struct timeval tmVal;
    tmVal.tv_sec = 0;
    tmVal.tv_usec = 0;
    gettimeofday(&tmVal, NULL);

    printf("local time:[ %04d-%02d-%02d %02d:%02d:%02d.%03d ]\n",
            (int)(1900 + curTm->tm_year), (int)(1 + curTm->tm_mon),
            (int)curTm->tm_mday,(int)curTm->tm_hour, (int)curTm->tm_min,
            (int)curTm->tm_sec,(int)tmVal.tv_usec/1000);

    return 0;
}
