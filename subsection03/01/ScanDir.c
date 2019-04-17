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
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "ScanDir.h"


/*
 *
 *
 */
int ScanDir(const char *usrDir, scanDir_t *usrScan)
{
    DIR *pDir = NULL ;
    struct dirent *pDirent = NULL;
    struct stat fileStat = {};
    char fullPath[PATH_MAX] = {};


    if (NULL == usrDir || NULL == usrScan)
        return -1;

    //clear the usrScan
    memset(usrScan, 0, sizeof(scanDir_t));

    snprintf(usrScan->path, MAX_NAME_LEN,"%s",usrDir);
    pDir = opendir(usrDir);
    if(NULL == pDir) {
        printf("%s open dir:%s %s\n",__func__, usrDir, strerror(errno));
        return -2;
    }


    //start scan dir
    for(pDirent = readdir(pDir); NULL != pDirent; pDirent = readdir(pDir))
    {
        if (strcmp(pDirent->d_name, ".") == 0 
                || strcmp(pDirent->d_name, "..") == 0) {
            continue;
        }

        memset(fullPath,0,sizeof(fullPath));
        sprintf(fullPath, "%s/%s", usrDir, pDirent->d_name);

        //printf("fullPath:%s\n",fullPath);
        if (stat(fullPath, &fileStat) < 0) {
            printf("%s stat %s %s\n",__func__, fullPath, strerror(errno));
            goto ERROR1;
        }

        //this is a subdir
        if (S_ISDIR(fileStat.st_mode)) {
            scanDir_t tmpScan = {};
            int ret = ScanDir(fullPath, &tmpScan);
            if ( 0 == ret) {
                //scan subdir ok, save  the data
                usrScan->totalSize += tmpScan.totalSize;
                usrScan->largeFileNum += tmpScan.largeFileNum;
            }
            else
                printf("%s scan subdir:%s failed:%s\n",__func__,
                        fullPath,strerror(errno));
        }
        //this is a file
        if (S_ISREG(fileStat.st_mode)) {
            //save the data
            usrScan->totalSize += fileStat.st_size;
            if(fileStat.st_size > LARGE_FILE_SIZE)
                usrScan->largeFileNum += 1;
        }


    }

    closedir(pDir);
    return 0;

ERROR1:
    closedir(pDir);
ERROR:
    return -1;
}


int OutShow(const scanDir_t *usrScan)
{
    if (NULL == usrScan)
        return -1;

    printf("totalSize:%lld\nlargeFileNum:%d\n",
           usrScan->totalSize, usrScan->largeFileNum);
    return 0;
}
