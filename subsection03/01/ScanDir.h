/*
 * file:        ScanDir.h
 * func:        scan dir, count file size
 * author:      liuxueneng@airFly
 * date:        20190417
 *
 */

#ifndef SCAN_DIR_H
#define SCAN_DIR_H

/*this is for C++ call*/
#ifdef __cplusplus
extern "C" {
#endif

//large file size 1M
#define LARGE_FILE_SIZE (1 << 20)
#define MAX_NAME_LEN    (256)


    typedef struct scanDir_s {
        char path[MAX_NAME_LEN];
        long long totalSize;
        int largeFileNum;
    }scanDir_t;


    /*open and read sortFile*/
    int ScanDir(const char *dir, scanDir_t *usrScan);

    /*out put the data*/
    int OutShow(const scanDir_t *usrScan);

/*this is for C++ call*/
#ifdef __cplusplus
}
#endif

#endif // SCAN_DIR_H
