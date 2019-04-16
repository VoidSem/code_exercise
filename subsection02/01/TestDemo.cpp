/*
 * file:        strSortDemo
 * func:        sort strings
 * author:      liuxueneng@airfly
 * date:        20190416
 *
 */

#include "StringSort.h"

#define     INPUT_FILE     "./input.txt"

using namespace std;

int main()
{
    StringSort usrSort(INPUT_FILE);

    usrSort.InitSort();

    usrSort.DoSortDown();

    usrSort.OutShow();

    return 0;
}
