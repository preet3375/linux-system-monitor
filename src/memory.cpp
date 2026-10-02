#include "memory.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

MemoryStats getMemoryStats(){
    MemoryStats m1{};

    ifstream inputfile("/proc/meminfo");

    if (!inputfile) {
        cout << "Failed to open /proc/meminfo" << endl;
        return MemoryStats{};
    }

    string s;

    while(getline(inputfile, s)){
        if(s.find("MemTotal") != string::npos){
            stringstream str(s);

            string label;
            long long value;
            string unit;

            str >> label >> value >> unit;

            m1.total = value;
        }

        if(s.find("MemAvailable") != string::npos){
            stringstream str(s);

            string label;
            long long value;
            string unit;

            str >> label >> value >> unit;

            m1.available = value;
        }
    }   
    return m1;
}
 


