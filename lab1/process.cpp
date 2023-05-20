#include "process.h"
using namespace std;

bool operator<(const Process& p1, const Process& p2) {
    // 按照优先级从高到低排序，如果优先级相同则按到达时间从早到晚排序
    if (p1.priority == p2.priority) {
        return p1.arrive_time < p2.arrive_time;
    } else {
        return p1.priority > p2.priority;
    }
}


