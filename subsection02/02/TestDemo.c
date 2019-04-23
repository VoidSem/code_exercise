/*
 * Name:        testDemo.c
 * DestroySort: sort Chinese strings
 * Author:      liuxueneng@airfly
 * Date:        20190417
 * Modify:      20190423
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

//usr func head
#include "StringSort.h"

int main(int argc, char **argv)
{

    strInf_t usrStr = {};
    char *file = NULL;
    int ret = -1;

    if (argc < 2) {
        printf("Please input file name\n");
        return -1;
    }

    file = argv[1];
    //check the file
    if(access(file, R_OK)) {
        printf("file:%s access filed\n", file);
        return -2;
    }

    //set env language code
    setlocale (LC_ALL, "zh_CN.UTF-8");

    if(ret < InitSort(file, &usrStr) < 0 )
    {
        printf("init sort failed\n");
        return -3;
    }

    DoSortUp(&usrStr);

    OutShow(&usrStr);

    //attenticon destroy
    DestroySort(&usrStr);

    return 0;
}
