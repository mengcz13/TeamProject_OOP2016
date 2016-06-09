/*
* @Author: mengcz13
* @Date:   2016-06-09 11:00:36
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-09 11:06:49
*/

#include <iostream>
#include "Ttree.h"

using namespace std;

int main(){
    Task* taskvec = new Task[4];
    for (int i = 0; i < 4; ++i) {
        taskvec[i].X = taskvec[i].Y = taskvec[i].T = 1;
    }
    Ttree ttree(taskvec, 4);
    ttree.pack();
    return 0;
}