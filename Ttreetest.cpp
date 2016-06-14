/*
* @Author: mengcz13
* @Date:   2016-06-09 11:00:36
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-14 20:08:48
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


    // Task* taskvec = new Task[4];
    // for (int i = 0; i < 4; ++i) {
    //     taskvec[i].X = taskvec[i].Y = taskvec[i].T = 1;
    // }
    // taskvec[1].Y = 2;
    // taskvec[2].X = 2;
    // taskvec[2].T = 2;
    // taskvec[3].T = 2;
    // Ttree ttree(taskvec, 4);
    // double pack = ttree.pack();
    // cout << pack << endl;
    return 0;
}