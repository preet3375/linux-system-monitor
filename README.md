# Linux System Monitor

A lightweight Linux system monitoring tool written in C++ that reads system and process statistics from the Linux `/proc` filesystem and displays them in a live terminal dashboard.

## Features

- **CPU Monitoring:** Displays overall system CPU utilization.
- **Memory Monitoring:** Displays total, used, and available memory, along with memory usage percentage.
- **Process Monitoring:** Lists running processes with their PID, parent PID (PPID), state, name, CPU usage, and memory usage.
- **Process Sorting:** Sorts processes by CPU usage or memory consumption.
- **Live Dashboard:** Refreshes system statistics periodically in the terminal.
- **Graceful Shutdown:** Restores the terminal screen when the monitor exits normally through `Ctrl+C`.

## Technologies

- C++
- Linux `/proc` filesystem
- C++ Standard Library
- `std::filesystem`
- Git and GitHub

## Project Structure

```text
linux-system-monitor/
├── main.cpp
├── cpu.h
├── cpu.cpp
├── memory.h
├── memory.cpp
├── process.h
├── process.cpp
└── README.md
```

## Requirements

- Linux operating system
- C++ compiler supporting C++17
- Standard C++ library

## Build

Compile the project from its source directory:

```bash
g++ -std=c++17 main.cpp cpu.cpp memory.cpp process.cpp -o monitor
```

## Usage

Run the monitor with the default CPU sorting:

```bash
./monitor
```

Sort processes by CPU usage:

```bash
./monitor cpu
```

Sort processes by memory usage:

```bash
./monitor memory
```

Press `Ctrl+C` to stop the monitor.

## How It Works

The program reads system statistics from `/proc/stat` and `/proc/meminfo`. It discovers processes through `/proc`, reads process information, and estimates CPU utilization by comparing statistics collected across consecutive snapshots.

## Project Status

Core monitoring features implemented. Further improvements, testing, and documentation are ongoing.