#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

int main(){

    ifstream inputfile("/proc/meminfo");

    if (!inputfile) {
        cout << "Failed to open /proc/meminfo" << endl;
        return 1;
    }

    string s;

    long long totalMemory = 0;
    long long availableMemory = 0;

    while(getline(inputfile, s)){
        if(s.find("MemTotal") != string::npos){
            stringstream str(s);

            string label;
            long long value;
            string unit;

            str >> label >> value >> unit;

            totalMemory = value;
        }

        if(s.find("MemAvailable") != string::npos){
            stringstream str(s);

            string label;
            long long value;
            string unit;

            str >> label >> value >> unit;

            availableMemory = value;
        }
    }

    long long used = totalMemory - availableMemory;
    long double usage = ((long double)used/totalMemory)*100; 

    cout << "Total Memory: " << totalMemory << " kB" << endl;
    cout << "Available Memory: " << availableMemory << " kB" << endl;
    cout << "Used Memory: " << used << " kB" << endl;
    cout << "Memory Usage: " << usage << "%" << endl;

    return 0;
}