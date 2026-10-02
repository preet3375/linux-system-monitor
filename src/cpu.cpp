#include "cpu.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

CpuStats getCpuStats(){

    CpuStats c1{};

    ifstream inputfile("/proc/stat");

    if (!inputfile) {
        cout << "Failed to open /proc/stat" << endl;
        return CpuStats{};
    }

    string s;

    while(getline(inputfile, s)){
        if(s.find("cpu ") == 0){
            stringstream str(s);
            string label;

            str >> label >> c1.user >> c1.nice >> c1.system
                >> c1.idle >> c1.iowait >> c1.irq
                >> c1.softirq >> c1.steal;

            break;
        }
    }

    return c1;
}