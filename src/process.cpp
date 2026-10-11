#include "process.h"
#include "cpu.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<thread>
#include<chrono>

using namespace std;

ProcessCpuStat getProcessCpuStat(long long pid){

    string path = "/proc/" + to_string(pid) + "/stat";
    ifstream inputfile(path);

    if(!inputfile){
        return {0,0};
    }

    string line;
    getline(inputfile,line);

    size_t closingParen = line.rfind(')');

    string remaining = line.substr(closingParen + 2);

    stringstream str(remaining);

    string state;
    str>>state;

    long long value;

    ProcessCpuStat p1{};

    for(int i=4;i<=15;i++){
        str>>value;

        if(i==14){
            p1.utime=value;
        }
        if(i==15){
            p1.stime=value;
        }
    }
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

    return p;
}

// int main(){

//     ProcessCpuStat p = getProcessCpuStat(1);
//     CpuStats first = getCpuStats();

//     this_thread::sleep_for(chrono::seconds(1));

//     ProcessCpuStat p2 = getProcessCpuStat(1);
//     CpuStats second = getCpuStats();

//     long long firstNonIdle = first.user + first.nice + first.system + first.irq + first.softirq + first.steal;
//     long long firstIdleTime = first.idle + first.iowait;
//     long long firstTotalTime = firstNonIdle + firstIdleTime;

//     long long secondNonIdle = second.user + second.nice + second.system + second.irq + second.softirq + second.steal;
//     long long secondIdleTime = second.idle + second.iowait;
//     long long secondTotalTime = secondNonIdle + secondIdleTime;
        
//     long long totalDelta = secondTotalTime - firstTotalTime;

//     long double cpuUsage = calculateProcessCpuUsage(p,p2,totalDelta);
//     cout << "CPU Usage : " << cpuUsage <<"%"<<endl; 



//     return 0;
// }