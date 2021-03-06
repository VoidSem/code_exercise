/*
 * Name:        StringSort.h
 * Description:        sort string
 * Author:      liuxueneng@airFly
 * Date:        20190417
 * Modify:      20190423
 */

#ifndef STRING_SORT_H
#define STRING_SORT_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_LINE    (128)
#define MAX_NAME_LEN    (256)
#define BUF_SIZE        (1024)

    typedef struct string_s {
        char            *str;
        unsigned int    len;
    }string_t;

    typedef struct stringInfo_s {
        unsigned int    strNum;
        char            fileName[MAX_NAME_LEN];
        char            *dataBuf;
        string_t        *strHead;
        long            fileSize;
    }strInf_t;

    /*open and read sortFile*/
    int InitSort(const char *file, strInf_t *strInf);

    /*sort strings up*/
    void DoSortUp(const strInf_t *strInf);

    /* traverse file string*/
    void OutShow(const strInf_t *strInf);

    /* free source */
    void DestroySort(strInf_t *strInf);

#ifdef __cplusplus
}
#endif

#endif // STRING_SORT_H
