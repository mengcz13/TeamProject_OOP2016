/*
* @Author: mengcz13
* @Date:   2016-06-09 11:00:36
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-15 11:24:44
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Ttree.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 6) {
        cout << "Usage: ./main <inputfile> <outputfile> <initial temperature> <final temperature> <alpha>" << endl;
        return -1;
    }
    srand(time(NULL));
    Placer placer(argv[1], argv[2], atof(argv[3]), atof(argv[4]), atof(argv[5]));
    return 0;
}