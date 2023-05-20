#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using namespace std;

class Process {
public:
    string name; // 进程名称
    int arrive_time; // 到达时间
    int service_time; // 服务时间,运行时间
    int priority; // 优先级

    int start_time; // 开始时间
    int finish_time; // 完成时间
    int wait_time;
    int init_service_time;
 
    Process(const std::string& name, int arrive_time, int service_time, int priority): name(name), arrive_time(arrive_time),service_time(service_time), priority(priority) {};

    friend bool operator<(const Process& p1, const Process& p2);

    // 重载运算符 <，按到达时间和优先级排序
    bool operator<(const Process& other) const {
        if (arrive_time == other.arrive_time) {
            return priority < other.priority;
        }
        return arrive_time < other.arrive_time;
    }

};

#endif
