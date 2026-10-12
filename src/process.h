#pragma once
#include<string>

struct ProcessCpuStat{
    long long utime;
    long long stime;
    char state;
};

struct Process {
    long long pid;
    std::string name;
    long long memory;
    long double cpu;
    long long ppid;
    char state;
};

ProcessCpuStat getProcessCpuStat(long long pid);

long double calculateProcessCpuUsage( ProcessCpuStat first, ProcessCpuStat second, long long totalDelta);

Process getProcessInfo(long long pid);