/*
 * file:        TestGetTime.c
 * func:        get time test demo
 * author:      liuxueneng@airfly
 * date:        20190417
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "GetTime.h"



int main(int argc, char **argv)
{

    int ret = -1;
    long sysTime = GetUnixTime();
    printf("Unix  time:[%ld]\n", sysTime);


#if 1
    char timeBuf[BUF_SIZE] = {};
    ret = GetLocalTime(timeBuf, BUF_SIZE);
    if(ret < 0)
        return -1;
    printf("local time:[%s]\n",timeBuf);
#else

#endif
    return 0;
}
