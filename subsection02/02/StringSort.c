/*
 * Name:        StringSort.c
 * Description: sort chinese string
 * Author:      liuxueneng@airFly
 * Date:        20190417
 * Modify:      20190423
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include "StringSort.h"

 /* cmp function */
int CmpUpString(const void * strA, const void * strB)
{
    return strcoll(((string_t *)strA)->str, ((string_t *)strB)->str);
}

/* read and save the file data*/
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
    memset(strInf, 0, sizeof(strInf_t));

    snprintf(strInf->fileName, MAX_NAME_LEN, "%s", file);
    FILE *fp = fopen(file, "r+");
    if (fp == NULL) {
        printf("can not open file %s\n", file);
        goto ERROR;
    }

    //get the file size
    fseek(fp, 0L, SEEK_END);
    long size = (long)ftell(fp);

    char *bufP = (char *)malloc(size);
    if (NULL == bufP) {
        goto ERROR1;
    }

    //init usr data
    strInf->dataBuf = bufP;
    strInf->fileSize = size;

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


/* sort strings down */
void DoSortUp(const strInf_t *strInf)
{
    qsort((void *)strInf->strHead, strInf->strNum,
            (sizeof(string_t)),CmpUpString);
}

/* traverse file string*/
void OutShow(const strInf_t *strInf)
{
    if (NULL == strInf)
        return;

    int i = 0;
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
