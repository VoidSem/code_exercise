/*
 * file:        testDemo.c
 * func:        scan dir file
 * author:      liuxueneng@airfly
 * date:        20190417
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ScanDir.h"



int main(int argc, char **argv)
{

    scanDir_t usrScan = {};
    char *dir= NULL;
    int ret = -1;

    if (argc < 2) {
        printf("parameters wrong\n");
        return -1;
    }

    dir = argv[1];
    //check the file
    if(access(dir, R_OK)) {
        printf("dir:%s access filed\n", dir);
        return -2;
    }


    ret = ScanDir(dir, &usrScan);
    if(ret < 0 )
    {
        printf("scan dir:%s failed\n", dir);
        return -3;
    }

    OutShow(&usrScan);

    return 0;
}
