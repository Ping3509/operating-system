//2205112E39李萍
#include <iostream>
#include <vector>

using namespace std;

const int MAX_PROCESS_NUM = 10; // 最大进程数
const int MAX_RESOURCE_NUM = 10; // 最大资源种类数

int available[MAX_RESOURCE_NUM]; // 可用资源数量
int allocation[MAX_PROCESS_NUM][MAX_RESOURCE_NUM]; // 分配给进程的资源数量
int need[MAX_PROCESS_NUM][MAX_RESOURCE_NUM]; // 进程还需要的资源数量
int max_need[MAX_PROCESS_NUM][MAX_RESOURCE_NUM]; // 进程最大需要的资源数量
bool finish[MAX_PROCESS_NUM]; // 进程是否完成

int n, m; // 进程数和资源数

// 检查是否满足安全性条件
bool isSafe(vector<int>& safe_sequence) {
    vector<int> work(available, available + m);
    vector<bool> finish_flag(n, false);

    int cnt = 0; // 已经完成的进程数量

    while (cnt < n) {
        bool flag = false; // 是否找到一个进程可以完成
        for (int i = 0; i < n; i++) {
            if (!finish_flag[i]) {
                bool enough = true; // 进程 i 是否有足够的资源可以完成
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        enough = false;
                        break;
                    }
                }

                if (enough) {
                    finish_flag[i] = true;
                    cnt++;
                    flag = true;
                    safe_sequence.push_back(i);
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                }
            }
        }

        if (!flag) {
            return false; // 没有找到一个进程可以完成，说明不安全
        }
    }

    return true;
}

int main() {
    cout << "请输入进程数和资源数：";
    cin >> n >> m;

    cout << "请输入每种资源的可用数量：";
    for (int i = 0; i < m; i++) {
        cin >> available[i];
    }

    cout << "请输入每个进程的最大需求量：";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> max_need[i][j];
            need[i][j] = max_need[i][j];
        }
    }

    cout << "请输入每个进程已分配的资源量：";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> allocation[i][j];
            need[i][j] -= allocation[i][j];
        }
    }

    vector<int> safe_sequence;
    if (isSafe(safe_sequence)) {
        cout << "存在安全序列：";
        for (int i = 0; i < safe_sequence.size(); i++) {
            cout << safe_sequence[i] << " ";
        }
        cout << endl;
    } else {
        cout << "不存在安全序列！" << endl;
    }
}