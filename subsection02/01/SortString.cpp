/*
 * Name:        strSortDemo
 * Description: sort strings
 * Author:      liuxueneng@airfly
 * Date:        20190416
 * Modify:      20190423
 */

#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>

using namespace std;

int main()
{
    vector <string> inStrVector;
    ifstream inFile;
    string line;

    inFile.open("./input.txt", ios::in);
    if (!inFile.is_open()) {
        cerr << "input file is not open " << endl;
        return -1;
    }
    while(getline(inFile, line)) {
        inStrVector.push_back(line);
    }
    inFile.close();
    sort(inStrVector.begin(), inStrVector.end(), greater<string>());
    for(auto i : inStrVector) {
        cout<< i <<endl;
    }
    inStrVector.clear();

}
