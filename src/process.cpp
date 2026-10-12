#include "process.h"
#include "cpu.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

ProcessCpuStat getProcessCpuStat(long long pid){

    string path = "/proc/" + to_string(pid) + "/stat";
    ifstream inputfile(path);

    if(!inputfile){
        return {0,0,'?'};
    }

    string line;
    if(!getline(inputfile,line)){
        return {0,0,'?'};
    }

    size_t closingParen = line.rfind(')');

    if(closingParen == string::npos){
        return {0,0,'?'};
    }

    string remaining = line.substr(closingParen + 2);

    stringstream str(remaining);

    char state;
    str>>state;

    if(!(str >> state)){
        return {0, 0, '?'};
    }

    long long value;

    ProcessCpuStat p1{};

    for(int i=4;i<=15;i++){
        if(!(str >> value)){
            return {0, 0, '?'};
        }

        if(i==14){
            p1.utime=value;
        }
        if(i==15){
            p1.stime=value;
        }
    }
    p1.state = state;

    return p1;
}

long double calculateProcessCpuUsage(ProcessCpuStat first, ProcessCpuStat second, long long totalDelta){
    long long firstProcessTime  = first.utime + first.stime;
    long long secondProcessTime = second.utime + second.stime;

    long long processDelta = secondProcessTime - firstProcessTime;

    long double usage = ((long double)processDelta / totalDelta) * 100;
    return usage;
}

Process getProcessInfo(long long pid) {
    Process p{};

    string statusPath = "/proc/" + to_string(pid) + "/status";

    ifstream inputfile(statusPath);

    if(!inputfile){
        return {};
    }

    string s;
    string processName;
    long long ppid = 0;
    long long memory = 0;

    while(getline(inputfile, s)){

        stringstream str(s);

        if(s.find("Name:") == 0){
            string label;
            str >> label >> processName;
        }
        else if(s.find("PPid:") == 0){
            string label;
            str >> label >> ppid;
        }
        else if(s.find("VmRSS:") == 0){
            string label;
            str >> label >> memory;
        }
    }
                
    p.pid = pid;
    p.name = processName;
    p.memory = memory;
    p.ppid = ppid;

    ProcessCpuStat stat = getProcessCpuStat(pid);
    p.state = stat.state;

    return p;
}