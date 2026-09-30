#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<thread>
#include<chrono>
using namespace std;

struct CpuStats{
    long long user;
    long long nice;
    long long system;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;
    long long steal;
};

CpuStats getCpuStats(){
    
    CpuStats c1{};

    ifstream inputfile("/proc/stat");

    if (!inputfile) {
        cout << "Failed to open /proc/stat" << endl;
        return CpuStats{};
    }

    string s;
    while(getline(inputfile,s)){
        if(s.find("cpu ") == 0){
            stringstream str(s);
            string label;

            
            str >> label >> c1.user >> c1.nice >> c1.system >> c1.idle >> c1.iowait >> c1.irq >> c1.softirq >> c1.steal;
            break;
        }
    }

    return c1;
};

int main(){

    CpuStats first = getCpuStats();
    while(true){
        this_thread::sleep_for(chrono::seconds(1));

        CpuStats second = getCpuStats();

        long long firstnonIdle = first.user + first.nice + first.system + first.irq + first.softirq + first.steal;
        long long firstidleTime = first.idle + first.iowait;
        long long firsttotalTime = firstnonIdle + firstidleTime;

        long long secondnonIdle = second.user + second.nice + second.system + second.irq + second.softirq + second.steal;
        long long secondidleTime = second.idle + second.iowait;
        long long secondtotalTime = secondnonIdle + secondidleTime;

        long long totalDelta = secondtotalTime - firsttotalTime;
        long long idleDelta = secondidleTime - firstidleTime;

        long double cpuUsage = ((long double)(totalDelta - idleDelta)/totalDelta) * 100;
        cout << "CPU Usage: " << cpuUsage << "%" << endl;

        first = second;
    }
    return 0;
}