#include <iostream>
#include <algorithm>
#include <queue>
#include "process.h"
#include "scheduler.h"

using namespace std;
//冒泡排序算法（每次找到最大的放在末尾）
void sortWithArriveTime(vector<Process>& pro){
    int num = pro.size();
    for(int i=0;i<num;i++){
        for(int j= 0;j<num-i;j++){
            if(pro[j].arrive_time>pro[j+1].arrive_time){
                Process temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}
//先来先服务调度算法
void FCFS(vector<Process>& processes) {

    // 输出格式化的分隔线和算法名称
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<"先来先服务调度算法";
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<endl;
    // 输出调度信息的表头
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");

    //按照进入顺序排序
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrive_time < p2.arrive_time;
    });

    // 初始化当前时间为第一个进程到达的时间
    int time = processes[0].arrive_time;

    int n=processes.size();
    float sum_T_time = 0;//平均周转时间
    float sum_QT_time = 0;//带权平均周转时间

    // 对于每个进程，计算其调度信息并输出
    for (const Process& p : processes) {
        // 如果当前时间小于进程的到达时间，将当前时间更新为进程的到达时间
        if (time < p.arrive_time) {
            time = p.arrive_time;
        }
        // 计算进程的完成时间、周转时间和带权周转时间
        int done_time = time + p.service_time;
        int T_time = done_time - p.arrive_time;
        float QT_time = T_time / (p.service_time + 0.0);
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        // 输出进程的调度信息
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",p.name.c_str(),p.arrive_time,p.service_time,time,done_time,T_time,QT_time);
        // 将当前时间更新为进程的完成时间
        time += p.service_time;
    }
    // 输出平均周转时间和平均带权周转时间
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(n+0.0),sum_QT_time/(n+0.0));
}


// 短进程优先调度算法
void SJF(vector<Process>& processes) {
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<"短进程优先调度算法";
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<endl;
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");

    // //按照运行时间排序
    // sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
    //     return p1.service_time < p2.service_time;
    // });
    
    int n = processes.size();
    int time = processes[0].arrive_time;
    float sum_T_time = 0, sum_QT_time = 0;

    int remaining_processes = processes.size();
    int index = 0;
    vector<Process> ready_queue; // 就绪队列
    while (remaining_processes > 0) {
        // 加入就绪队列
        while (index < processes.size() && processes[index].arrive_time <= time) {
            ready_queue.push_back(processes[index++]);
        }
        // 按服务时间排序
        sort(ready_queue.begin(), ready_queue.end(), [](const Process& p1, const Process& p2) {
            return p1.service_time < p2.service_time;
        });
        if (ready_queue.empty()) {
            // CPU空闲，等待下一个进程到达
        time = processes[index].arrive_time;
        continue;
        }
        // 执行就绪队列中服务时间最短的进程
        Process p = ready_queue.front();
        ready_queue.erase(ready_queue.begin());
        int done_time = time + p.service_time;
        int T_time = done_time - p.arrive_time;
        float QT_time = T_time / (p.service_time + 0.0);
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",p.name.c_str(),p.arrive_time,p.service_time,time,done_time,T_time,QT_time);
        time += p.service_time;
        remaining_processes--;
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(n+0.0),sum_QT_time/(n+0.0));
}

// 高优先级优先调度算法
void HPF(vector<Process>& processes) {
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<"高优先级优先调度算法";
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<endl;
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");

    int n = processes.size();
    int time = processes[0].arrive_time;
    float sum_T_time = 0, sum_QT_time = 0;

    //按照优先级排序
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.priority > p2.priority;
    });

    // 处理进程
    for (const Process& p : processes){

        if (time < p.arrive_time) {
            time = p.arrive_time;
        }

        int done_time = time + p.service_time;
        int T_time = done_time - p.arrive_time;
        float QT_time = T_time / (p.service_time + 0.0);

        sum_T_time += T_time;
        sum_QT_time += QT_time;

        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",p.name.c_str(),p.arrive_time,p.service_time,time,done_time,T_time,QT_time);
        time += p.service_time;

        //按优先级排序
        sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
            return p1.priority > p2.priority;
        });
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n", sum_T_time / n, sum_QT_time / n);
   
    
}

// 高响应比优先调度算法
void HRRN(vector<Process>& processes) {
    for (int i = 0; i <= 14; i++) cout << '-';
    cout << "高响应比优先调度算法";
    for (int i = 0; i <= 14; i++) cout << '-';
    cout << endl;
    cout << "进程 到达时间 服务时间 开始时间 完成时间 周转时间 带权周转时间 响应比" << endl;

    int n = processes.size();
    int time = processes[0].arrive_time;
    float sum_T_time = 0, sum_QT_time = 0, sum_response_ratio = 0; // 新增：记录总响应比
    float response_ratio = 0; // 新增：记录每个进程的响应比

    //按照进入顺序排序
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrive_time < p2.arrive_time;
    });

    queue<Process> process_queue;
    process_queue.push(processes[0]);
    for (int i = 1; i < processes.size(); i++) {
        Process p = processes[i];
        while (!process_queue.empty() && (p.arrive_time - time) > 0) {
            Process top_p = process_queue.front();
            process_queue.pop();
            int done_time = time + top_p.service_time;

            int T_time = done_time - top_p.arrive_time;
            float QT_time = T_time / (top_p.service_time + 0.0);

            // 计算响应比
            response_ratio = (T_time + top_p.service_time) / (p.arrive_time - top_p.arrive_time + 0.0);
            sum_response_ratio += response_ratio; // 新增：记录总响应比

            cout << top_p.name << "\t" << top_p.arrive_time << "\t" << top_p.service_time << "\t" << time << "\t" << done_time << "\t"
                << T_time << "\t" << QT_time << "\t" << response_ratio << endl;
            time += top_p.service_time;
        }
        if (process_queue.empty()) {
            time = p.arrive_time;
        }
        process_queue.push(p);
    }
    while (!process_queue.empty()) {
        Process top_p = process_queue.front();
        process_queue.pop();
        int done_time = time + top_p.service_time;
        int T_time = done_time - top_p.arrive_time;

        float QT_time = T_time / (top_p.service_time + 0.0);
        float response_ratio = T_time / (done_time - top_p.arrive_time + 0.0);
        sum_T_time += T_time;
        sum_QT_time += QT_time;

        // 计算响应比
        response_ratio = (T_time + top_p.service_time) / (time - top_p.arrive_time + top_p.service_time + 0.0);
        sum_response_ratio += response_ratio; // 新增：记录总响应比
        cout << top_p.name << "\t" << top_p.arrive_time << "\t" << top_p.service_time << "\t" << time << "\t" << done_time << "\t"
            << T_time << "\t" << QT_time << "\t" << response_ratio << endl;
        time += top_p.service_time;
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\t平均响应比为%.2f\n", sum_T_time / n, sum_QT_time / n, sum_response_ratio / n);
}


// 时间片轮转调度算法
void RR(vector<Process>& processes, int time_slice) {
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<"时间片轮转调度算法";
    for(int i=0;i<=14;i++)cout<<'-';
    cout<<endl;
    cout << "进程 到达时间 服务时间 开始时间 完成时间 周转时间 带权周转时间" << endl;

    //按照进入顺序排序
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrive_time < p2.arrive_time;
    });
    
    queue<Process> ready_queue;
    float sum_TAT_time = 0, sum_WTAT_time = 0, sum_response_ratio = 0;
    int n = processes.size();
    int cur_time = processes[0].arrive_time;
    int cur_index = 0;
    int finished_count = 0;

    while (finished_count < n) {
        while (cur_index < n && processes[cur_index].arrive_time <= cur_time) {
            ready_queue.push(processes[cur_index]);
            cur_index++;
        }

        if (ready_queue.empty()) {
            cur_time = processes[cur_index].arrive_time;
            continue;
        }

        Process cur_pro = ready_queue.front();
        cur_pro.init_service_time = cur_pro.service_time;
        ready_queue.pop();
        int exec_time = min(time_slice, cur_pro.service_time);
        cur_pro.service_time -= exec_time;
        cur_time += exec_time;
        

        if (cur_pro.service_time <= 0) {
            finished_count++;
            int TAT_time = cur_time - cur_pro.arrive_time;
            float WTAT_time = TAT_time / (cur_pro.init_service_time + 0.0);
            float response_ratio = TAT_time / (cur_time - cur_pro.arrive_time + 0.0);
            sum_TAT_time += TAT_time;
            sum_WTAT_time += WTAT_time;
            sum_response_ratio += response_ratio;

            cout << cur_pro.name << "\t" << cur_pro.arrive_time << "\t" << cur_pro.init_service_time << "\t" << cur_time - cur_pro.service_time << "\t" << cur_time << "\t"
                    << TAT_time << "\t" << WTAT_time << "\t" <<  endl;
        } else {
            ready_queue.push(cur_pro);
        }

    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n", sum_TAT_time / n, sum_WTAT_time / n);
}
