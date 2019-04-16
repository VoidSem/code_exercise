/*
 * file:        StringSort.cpp
 * func:        sort string
 * author:      liuxueneng@airfly
 * data:        20190416
 *
 *
 */

#include<algorithm>
#include "StringSort.h"

using namespace std;

StringSort::StringSort(const string &srcFile)
{
   sortFile = srcFile;
}

/*
* 
*
*/
void StringSort::InitSort()
{
    string line;

    inFile.open(sortFile, ios::in);
    if (inFile.is_open()) {
        while(getline(inFile, line)) {
            inStrVector.push_back(line);
        }
    }//dtor
    else
        cerr << "input file is not open " << endl;

}


void StringSort::DoSortDown()
{
    sort(inStrVector.begin(), inStrVector.end(), greater<string>());
}

void StringSort::DoSortUp()
{
    sort(inStrVector.begin(), inStrVector.end(), less<string>());
}

void StringSort::OutShow()
{
    for(auto i : inStrVector) {
        cout<< i <<endl;
    }
}


StringSort::~StringSort()
{
    if (inFile.is_open()) {
        inFile.close();
    }
    inStrVector.clear();
}
