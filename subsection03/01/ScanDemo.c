/*
 * Name:        testDemo.c
 * Description: scan dir file
 * Author:      liuxueneng@iairfly
 * Date:        20190417
 * Modify:      20190423
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ScanDir.h"

int main(int argc, char **argv)
{
    scanDir_t usrScan = {};

    if (argc < 2) {
        printf("Please input dir name\n");
        return -1;
    }

    if (ScanDir(argv[1], &usrScan) < 0) {
        printf("scan dir:%s failed\n", argv[1]);
        return -3;
    }

    OutShow(&usrScan);

    return 0;
}
