/*
 * file:        GetTime.h
 * func:        gettime
 * author:      liuxueneng@airFly
 * date:        20190417
 *
 */

#ifndef GET_TIME_H
#define GET_TIME_H

#define BUF_SIZE        (1024)

/*this is for C++ call*/
#ifdef __cplusplus
extern "C" {
#endif


    /*get unix time*/
    long GetUnixTime();

    /*
     * get local time
     * out:YYYY-MM-DD HH:MM:SS.fff
     *
     */
    int GetLocalTime(char *timeBuf, int bufLen);

    /*
     * unix time transform to local time
     * out:YYYY-MM-DD HH:MM:SS.fff
     *
     */
    int ToLocalTime(time_t *timeNow, char *timeBuf, int bufLen);

/*this is for C++ call*/
#ifdef __cplusplus
}
#endif

#endif // GET_TIME_H
