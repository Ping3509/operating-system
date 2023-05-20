#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "process.h"
using namespace std;
//先来先服务调度算法
void FCFS(vector<Process>& processes);
//短作业优先调度算法
void SJF(vector<Process>& processes);
//高优先权优先调度算法
void HPF(vector<Process>& processes);
//高响应比优先调度算法
void HRRN(vector<Process>& processes);
//时间片轮转调度算法
void RR(vector<Process>& processes, int time_slice);

#endif
