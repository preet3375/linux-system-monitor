#pragma once

struct MemoryStats{
    long long total;
    long long available;
};

MemoryStats getMemoryStats();