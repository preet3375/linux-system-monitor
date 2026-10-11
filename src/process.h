#pragma once
#include<string>

struct ProcessCpuStat{
    long long utime;
    long long stime;
};

struct Process {
    long long pid;
    std::string name;
    long long memory;
    long double cpu;
};

ProcessCpuStat getProcessCpuStat(long long pid);

long double calculateProcessCpuUsage( ProcessCpuStat first, ProcessCpuStat second, long long totalDelta);

Process getProcessInfo(long long pid);