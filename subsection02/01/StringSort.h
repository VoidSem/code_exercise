/*
 * file:        StringSort.h
 * func:        sort string
 * author:      liuxueneng@airFly
 * date:        20190416
 *
 */

#ifndef STRINGSORT_H
#define STRINGSORT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using std::string;
using std::vector;
using std::list;
using std::ifstream;


class StringSort
{
public:
    StringSort(const string &srcFile);

    /*open and read sortFile*/
    void InitSort();

    /*sort strings up*/
    void DoSortUp();

    /* sort strings down */
    void DoSortDown();

    /* traverse file string*/
    void OutShow();

    virtual ~StringSort();

protected:

private:
    string sortFile;
    vector <string> inStrVector;
    ifstream inFile;
};

#endif // STRINGSORT_H
