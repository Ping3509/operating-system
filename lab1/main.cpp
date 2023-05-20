//2205112E39李萍
#include "process.h"
#include "scheduler.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    // 测试数据
    vector<Process> processes;
    //到达时间 服务时间 优先级
    processes.emplace_back("P1", 0, 5, 3);
    processes.emplace_back("P2", 2, 3, 4);
    processes.emplace_back("P3", 3, 1, 1);
    processes.emplace_back("P4", 4, 2, 2);
    processes.emplace_back("P5", 5, 4, 5);

    FCFS(processes);
    SJF(processes);
    HPF(processes);
    HRRN(processes);
    RR(processes, 2);


    return 0;
}