/*
 * file:        testDemo.c
 * func:        sort strings
 * author:      liuxueneng@airfly
 * date:        20190417
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//usr func head
#include "StringSort.h"



int main(int argc, char **argv)
{

    strInf_t usrStr = {};
    char *file = NULL;
    int ret = -1;

    if (argc < 2) {
        printf("parameters wrong\n");
        return -1;
    }

    file = argv[1];
    //check the file
    if(access(file, R_OK)) {
        printf("file:%s access filed\n", file);
        return -2;
    }


    ret = InitSort(file, &usrStr);
    if(ret < 0 )
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
