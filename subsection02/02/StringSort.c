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
#include <sys/types.h>
#include <errno.h>

#include "StringSort.h"

#if 0
typedef struct string_s{
    char *str;
    unsigned int len;
}string_t;

typedef struct stringInfo_s{
    unsigned int num;
    char name[MAX_NAME_LEN];
    strint_t *strHead;
    long fileSize;
}strInf_t;
#endif

static char *GetMemory(long len)
{
    if(len <= 0)
        return NULL;

    //get memory space
    char *tmp= (char *)malloc((size_t)len);


    return tmp;
}

/*
 *
 */
static int CmpString(string_t strA, string_t strB)
{
    return strcmp(strA.str, strB.str);
}


/*
 *
 * forbid recursion
 *
 */
static void InsertSort(string_t *strHead, unsigned int strNum, sortOrder_t order)
{
    int i = 0;
    int j = 0;
    int k = 0;
    string_t *str  = strHead;
    string_t tmpStr  = {};

    if(NULL == str)
        return;

    //compare from the second element
    for (i = 1; i < strNum; ++i) {
        tmpStr = str[i];
        k = i - 1;

        while ((k >= 0) &&
                ((SORT_UP == order && (CmpString(tmpStr, str[k]) < 0)) ||
                 (SORT_DOWN == order && (CmpString(tmpStr, str[k]) > 0)))){
            str[k + 1] = str[k];
            --k;
        }

        str[k + 1] = tmpStr;
    }

}


/*
 * func:
 * 1 read and save the file data.
 * 2 init  the string pointer point to the databuf.
 *
 */
static int InitFileString(FILE *fp, strInf_t *strInf)
{
    int readByte = 0;
    char *readP = NULL;
    char *bufP = strInf->dataBuf;
    string_t *strP = NULL;
    unsigned  int num = DEFAULT_LINE;
    unsigned int tmpNum = strInf->fileSize >> 4;

    num = tmpNum < DEFAULT_LINE ? DEFAULT_LINE : tmpNum;

    strP = (string_t *)calloc(num, sizeof(string_t));
    if(NULL == strP) {
        printf("%s calloc failed\n",__func__);
        goto ERROR;
    }

    //save it
    strInf->strHead = strP;

    while (!feof(fp)) {
        //offset the bufP
        bufP += readByte;

        readP = fgets(bufP, BUF_SIZE, fp);
        if (NULL == readP) {
            break;
        }

        //point to the string
        strP->str = bufP;

        //save the string len
        readByte = strlen(bufP) + 1;
        strP->len= (unsigned int)readByte;

        ++strInf->strNum;
        if(strInf->strNum > num) {
            num += DEFAULT_LINE;
            strP = (string_t *)realloc(strInf->strHead, num * sizeof(string_t));
            if(NULL == strP) {
                printf("%s realloc failed\n",__func__);
                goto ERROR1;
            }

            //attention update the pointers
            strInf->strHead = strP;
            strP += strInf->strNum;
        }
        else {
            //offset the string pointer
            ++strP;
        }
    }

    if( !feof(fp))
        goto ERROR1;

    //resize the strings pointer
    strP = realloc(strInf->strHead, strInf->strNum * sizeof(string_t));
    if(NULL == strP) {
        printf("%s realloc failed\n",__func__);
        goto ERROR1;
    }
    strInf->strHead = strP;

    return 0;

ERROR1:
    if(strInf->strHead)
        free(strInf->strHead);
ERROR:
    return -1;
}

/*open and read sortFile*/
int InitSort(const char *file, strInf_t *strInf)
{

    if (NULL == file || access(file, F_OK)) {
        printf("parameters wrong exit\n");
        goto ERROR;
    }

    memset(strInf, 0, sizeof(strInf_t));

    //save the file name
    snprintf(strInf->fileName, MAX_NAME_LEN, "%s", file);
    FILE *fp = fopen(file, "r+");
    if (fp == NULL) {
        printf("can not open file %s\n", file);
        goto ERROR;
    }

    //get the file size
    fseek(fp, 0L, SEEK_END);
    long size = (long)ftell(fp);

    if(size <= 0) {
        goto ERROR1;
    }

    char *bufP = GetMemory(size);
    if (NULL == bufP) {
        goto ERROR1;
    }

    //init usr data
    strInf->dataBuf = bufP;
    strInf->fileSize = size;

    bufP = NULL;

    //attention  offset the fp
    rewind(fp);

    if (InitFileString(fp,strInf)) {
        printf("%s initFileString failed\n",__func__);
        goto ERROR2;
    }

    //success return
    fclose(fp);
    return 0;

ERROR2:
    free(strInf->dataBuf);
ERROR1:
    fclose(fp);
ERROR:
    return -1;
}

/*sort strings up*/
void DoSortUp(const strInf_t *strInf)
{
    InsertSort(strInf->strHead,strInf->strNum, SORT_UP);
}

/* sort strings down */
void DoSortDown(const strInf_t *strInf)
{
    InsertSort(strInf->strHead,strInf->strNum, SORT_DOWN);
}

/* traverse file string*/
void OutShow(const strInf_t *strInf)
{
    int i = 0;

    if (NULL == strInf)
        return;

    for (i = 0; i < strInf->strNum; ++i) {
        printf("%s", (strInf->strHead +i)->str);
    }

}

/* free memroy */
void DestroySort(strInf_t *strInf)
{
    if(NULL == strInf)
        return;

    free(strInf->dataBuf);
    free(strInf->strHead);
    strInf->strNum = 0;
}
