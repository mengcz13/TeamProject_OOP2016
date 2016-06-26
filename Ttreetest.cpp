/*
* @Author: mengcz13
* @Date:   2016-06-09 11:00:36
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-15 11:24:44
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Ttree.h"
#include "Display.h"

using namespace std;

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc == 6) {
        Placer placer(argv[1], argv[2], atof(argv[3]), atof(argv[4]), atof(argv[5]));
        showtask(argv[2]);
    }
    else if (argc == 4) {
        for (int i = 0; i < 10; i++) {
            stringstream ss;
            ss << i+1 << ".txt";
            Placer placer("test/input" + ss.str(), "output/output" + ss.str(), atof(argv[1]), atof(argv[2]), atof(argv[3]));
        }
    }
    else {
        cout << "Usage:\n";
        cout << "Test one case:\n./main <inputfile> <outputfile> <initial temperature> <final temperature> <alpha>" << endl;
        cout << "Load default test cases:\n./main <initial temperature> <final temperature> <alpha>" << endl;
        return -1;
    }
    
    
    return 0;
}