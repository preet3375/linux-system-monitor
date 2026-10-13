#include "cpu.h"
#include "memory.h"
#include "process.h"
#include<iostream>
#include<thread>
#include<chrono>
#include<iomanip>
#include<vector>
#include<cctype>
#include<algorithm>
#include<filesystem>
#include<unistd.h>
#include<unordered_map>
#include<csignal>

using namespace std;
volatile sig_atomic_t running = 1;
namespace fs = std::filesystem;

void handleSignal(int signal){
    running = 0;
}

int main(int argc, char* argv[]){

    CpuStats first = getCpuStats();

    vector<pair<long long , ProcessCpuStat>> firstProcessStats;

    for(const auto& entry : fs::directory_iterator("/proc")){

        if(entry.is_directory()){

            string name = entry.path().filename().string();

            if(!name.empty() && all_of(name.begin(), name.end(), ::isdigit)){

                long long pid = stoll(name);
                ProcessCpuStat stat = getProcessCpuStat(pid);

                firstProcessStats.emplace_back(pid, stat);
            }
        }
    }
    unordered_map<long long, ProcessCpuStat> previousStats;

    for(const auto& process : firstProcessStats){
        previousStats[process.first] = process.second;
    }

    bool sortByMemory = false;

    if(argc > 2){
        cerr << "Usage: " << argv[0] << " [cpu|memory]\n";
        cout << "\033[?1049l" << flush;
        return 1;
    }

    if(argc == 2){
        string mode = argv[1];

        if(mode == "memory"){
            sortByMemory = true;
        }
        else if(mode != "cpu"){
            cerr << "Usage: " << argv[0] << " [cpu|memory]\n";
            cout << "\033[?1049l" << flush;
            return 1;
        }
    }
    cout << "\033[?1049h";

    signal(SIGINT, handleSignal);

    while(running){

        this_thread::sleep_for(chrono::seconds(1));

        vector<pair<long long, ProcessCpuStat>> secondProcessStats;

        CpuStats second = getCpuStats();
        MemoryStats memory = getMemoryStats();

        long long memoryUsed = memory.total - memory.available;
        long double memoryUsage = 0.0L;

        if(memory.total > 0){
            memoryUsage = ((long double)memoryUsed / memory.total) * 100.0L;
        }

        long long firstNonIdle = first.user + first.nice + first.system + first.irq + first.softirq + first.steal;
        long long firstIdleTime = first.idle + first.iowait;
        long long firstTotalTime = firstNonIdle + firstIdleTime;

        long long secondNonIdle = second.user + second.nice + second.system + second.irq + second.softirq + second.steal;
        long long secondIdleTime = second.idle + second.iowait;
        long long secondTotalTime = secondNonIdle + secondIdleTime;

        long long totalDelta = secondTotalTime - firstTotalTime;
        long long idleDelta = secondIdleTime - firstIdleTime;

        long double cpuUsage = 0.0L;

        if(totalDelta > 0){
            cpuUsage = ((long double)(totalDelta - idleDelta) / totalDelta) * 100.0L;
        }

        vector<Process> processes;

        for(const auto& entry : fs::directory_iterator("/proc")){

            if(entry.is_directory()){

                string name = entry.path().filename().string();

                if(!name.empty() && all_of(name.begin(), name.end(), ::isdigit)){

                    long long pid = stoll(name);

                    Process p = getProcessInfo(pid);
                    
                    if(p.pid == 0){
                        continue;
                    }

                    processes.push_back(p);

                    ProcessCpuStat stat = getProcessCpuStat(pid);
                    secondProcessStats.emplace_back(pid,stat);
                }
            }
        }

        unordered_map<long long, size_t> processIndex;

        for(size_t i = 0; i < processes.size(); i++){
            processIndex[processes[i].pid] = i;
        }

        for(const auto& current : secondProcessStats){

            long long pid = current.first;
            ProcessCpuStat secondStat = current.second;

            auto it = previousStats.find(pid);
            
            if(it != previousStats.end()){
                ProcessCpuStat firstStat = it->second;

                long long processDelta =
                    (secondStat.utime + secondStat.stime) -
                    (firstStat.utime + firstStat.stime);

                if(processDelta < 0 || totalDelta <= 0){
                    continue;
                }
                if(totalDelta > 0){
                    long double processCpuUsage =
                        ((long double)processDelta / totalDelta) * 100;

                    auto processIt = processIndex.find(pid);

                    if(processIt != processIndex.end()){
                        processes[processIt->second].cpu = processCpuUsage;
                    }
                }
            }
        }

        sort(processes.begin(), processes.end(), [sortByMemory](const Process& a, const Process& b) {
            if(sortByMemory){
                return a.memory > b.memory;
            }
            return a.cpu > b.cpu;
        });

        cout << "\033[H\033[2J" << flush;
        
        cout << "========================================" << endl;
        cout << "        LINUX SYSTEM MONITOR            " << endl;
        cout << "========================================" << endl;

        cout << "CPU Usage        : " << fixed << setprecision(2) << cpuUsage << "%" << endl;
        cout << "Memory Usage     : " << fixed << setprecision(2) << memoryUsage << "%" << endl;
        cout << "Available Memory : " << memory.available << " kB" << endl;
        cout << "Used Memory      : " << memoryUsed << " kB" << endl;
        cout << "Total Memory     : " << memory.total << " kB" << endl;

        cout << "========================================" << endl;
        cout << (sortByMemory ? "Top Processes by Memory" : "Top Processes by CPU") << endl;
        cout << "========================================" << endl;

        cout << left
             << setw(10) << "PID"
             << setw(10) << "PPID"
             << setw(8) << "STATE"
             << setw(40) << "NAME"
             << setw(15) << "CPU"
             << setw(15) << "MEMORY"
             << endl;

        cout << string(90, '-') << endl;

        int count = 0;

        for(const auto& process : processes){

            cout << left
                 << setw(10) << process.pid
                 << setw(10) << process.ppid
                 << setw(8) << process.state
                 << setw(40) << process.name    
                 << fixed << setprecision(2)
                 << setw(15) << (to_string((double)process.cpu) + " %")
                 << setw(15) << (to_string(process.memory) + " kB")
                 << endl;

            count++;

            if(count == 10)
                break;
        }

        first = second;
        previousStats.clear();

        for(const auto& process : secondProcessStats){
            previousStats[process.first] = process.second;
        }
    }
    cout << "\033[?1049l" << flush;
    return 0;
}