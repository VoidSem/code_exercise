/*
 * file:        StringSort.c
 * func:        sort string
 * author:      liuxueneng@airFly
 * date:        20190417
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#include "GetTime.h"


/*
 *
 *
 */
long GetUnixTime(void)
{
    time_t timeNow;
    struct tm *curTime = NULL;

    time(&timeNow);
    //printf("timeNow:%ld\n",timeNow);

    return timeNow;
}

int ToLocalTime(time_t *timeNow, char *timeBuf, int bufLen)
{

    struct tm *curTm = localtime(timeNow);

    if (NULL == timeNow || NULL == timeBuf || bufLen <= 0)
        return -1;

    struct timeval tmVal;
    tmVal.tv_sec = 0;
    tmVal.tv_usec = 0;

    gettimeofday(&tmVal, NULL);

    snprintf(timeBuf, bufLen, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            (int)(1900 + curTm->tm_year), (int)(1 + curTm->tm_mon), (int)curTm->tm_mday,
            (int)curTm->tm_hour, (int)curTm->tm_min, (int)curTm->tm_sec, 
            (int)tmVal.tv_usec/1000);
    //printf("%s:%s\n", __func__, timeBuf);

    return 0;
}


int GetLocalTime(char *timeBuf, int bufLen)
{
    time_t timeNow;
    struct tm *curTm = NULL;

    if (NULL == timeBuf || bufLen <= 0)
        return -1;

    time(&timeNow);
    curTm = localtime(&timeNow);

    struct timeval tmVal;
    tmVal.tv_sec = tmVal.tv_usec = 0;
    gettimeofday(&tmVal, NULL);

    snprintf(timeBuf, bufLen, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            (int)(1900 + curTm->tm_year), (int)(1 + curTm->tm_mon), (int)curTm->tm_mday,
            (int)curTm->tm_hour, (int)curTm->tm_min, (int)curTm->tm_sec, 
            (int)tmVal.tv_usec/1000);
    //printf("%s:%s\n", __func__, timeBuf);

    return 0;

}
