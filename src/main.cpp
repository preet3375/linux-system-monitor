#include "cpu.h"
#include "memory.h"
#include<iostream>
#include<thread>
#include<chrono>
#include<iomanip>
using namespace std;

int main(){

    CpuStats first = getCpuStats();

    while(true){
        this_thread::sleep_for(chrono::seconds(1));

        CpuStats second = getCpuStats();
        MemoryStats memory = getMemoryStats();


        long long memoryUsed = memory.total - memory.available;
        long double memoryUsage = ((long double)memoryUsed/memory.total)*100; 


        long long firstNonIdle = first.user + first.nice + first.system + first.irq + first.softirq + first.steal;
        long long firstIdleTime = first.idle + first.iowait;
        long long firstTotalTime = firstNonIdle + firstIdleTime;

        long long secondNonIdle = second.user + second.nice + second.system + second.irq + second.softirq + second.steal;
        long long secondIdleTime = second.idle + second.iowait;
        long long secondTotalTime = secondNonIdle + secondIdleTime;

        long long totalDelta = secondTotalTime - firstTotalTime;
        long long idleDelta = secondIdleTime - firstIdleTime;

        long double cpuUsage = ((long double)(totalDelta - idleDelta)/totalDelta) * 100;


        cout << "\033[2J\033[H";
        
        cout << "========================================" << endl;
        cout << "        LINUX SYSTEM MONITOR            " << endl;
        cout << "========================================" << endl;

        cout << "CPU Usage        : " << fixed << setprecision(2) << cpuUsage << "%" << endl;
        cout << "Memory Usage     : " << fixed << setprecision(2) << memoryUsage << "%" << endl;
        cout << "Available Memory : " << memory.available << " kB" << endl;
        cout << "Used Memory      : " << memoryUsed << " kB" << endl;
        cout << "Total Memory     : " << memory.total << " kB" << endl;

        cout << "========================================" << endl;
        first = second;
    }

    return 0;
}