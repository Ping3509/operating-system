//2205112E39李萍

#include <iostream>
#include <iomanip>

using namespace std;
#define MaxNumber 100

class PartitionAlgorithm {
public:

    int FreePartition[MaxNumber];//空闲分区大小
    int PartitionNum;//空闲分区个数
    int ProcessNum;//进程个数
    int isAlgorithm;//算法选择

    //定义进程的数据结构
    typedef struct {
        int FirstPartition;//首次适应分区
        int BestPartition;//最佳适应分区
        int WorstPartition;//最坏适应分区
        int ProcessNeed;//进程需要的分区大小
    } Progress;

    Progress progress[MaxNumber];

    //输入空闲分区数、空闲的分区大小、进程数、进程需要的分区大小
    void Input() {
        cout << "请输入空闲分区个数：";
        cin >> PartitionNum;

        for (int i = 1; i <= PartitionNum; i++) {
            cout << "请输入空闲分区" << i << "大小：";
            cin >> FreePartition[i];
        }

        cout << "请输入进程个数：";
        cin >> ProcessNum;

        for (int i = 1; i <= ProcessNum; i++) {
            cout << "请输入进程" << i << "需要的分区大小：";
            cin >> progress[i].ProcessNeed;
        }

        cout << endl;

        InputAlgorithm();
    }

    //获取算法选择输入
    void InputAlgorithm() {
        cout << endl
             << "请选择想要先使用的算法（1-首次适应分区算法（FirstPartition），2-最佳适应分区算法（BestPartition），3-最坏适应分区算法（WorstPartition））：";
        cin >> isAlgorithm;
        IsAlgorithm();
    }

    //算法存储确认
    void IsAlgorithm() {
        switch (isAlgorithm) {
            case 1:
                cout << endl << "您选择的是1-首次适应分区算法（FirstPartition）" << endl;
                AlgorithmFirstPartition();
                break;
            case 2:
                cout << endl << "您选择的是2-最佳适应分区算法（BestPartition）" << endl;
                AlgorithmBestPartition();
                break;
            case 3:
                cout << endl << "您选择的是3-最坏适应分区算法（WorstPartition）" << endl;
                AlgorithmWorstPartition();
                break;
            default:
                cout << "算法值：" << isAlgorithm
                     << "有误,请重新输入正确的算法类型（1-首次适应分区算法（FirstPartition），2-最佳适应分区算法（BestPartition），3-最坏适应分区算法（WorstPartition））"
                     << endl;
                InputAlgorithm();
        }
    }

    //输出空闲分区剩余情况、各进程分配情况
    void Print(Progress pg[MaxNumber], int fp[MaxNumber]) {

        cout << endl << left << setw(15) << "进程分配情况如下表：" << endl;

        //分区状况
        cout << left << setw(15) << "";
        for (int i = 1; i <= PartitionNum; i++) {
            cout << right << setw(10) << "分区" << setw(2) << i;
        }
        cout << endl << left << setw(15) << "已分配";
        for (int i = 1; i <= PartitionNum; i++) {
            cout << right << setw(12) << FreePartition[i] - fp[i];
        }
        cout << endl << left << setw(15) << "可用";
        for (int i = 1; i <= PartitionNum; i++) {
            cout << right << setw(11) << fp[i];
        }
        cout << endl;

        //进程分配状况
        cout << left << setw(15) << "";
        for (int i = 1; i <= ProcessNum; i++) {
            cout << right << setw(10) << "进程" << setw(2) << i;
        }
        cout << endl << left << setw(15) << "进程个数";
        for (int i = 1; i <= ProcessNum; i++) {
            switch (isAlgorithm) {
                case 1:
                    cout << right << setw(12) << pg[i].FirstPartition;
                    break;
                case 2:
                    cout << right << setw(12) << pg[i].BestPartition;
                    break;
                case 3:
                    cout << right << setw(12) << pg[i].WorstPartition;
                    break;
            }
        }
        cout << endl;

    }
    //首次适应分区算法
    //调用FirstPartition算法进行调度计算
    void AlgorithmFirstPartition() {
        //复制进程
        Progress progressCopy[MaxNumber];
        for (int i = 1; i <= ProcessNum; i++) {
            progressCopy[i] = progress[i];
        }

        //复制空闲分区大小
        int freePartition[MaxNumber];//空闲分区大小
        for (int i = 1; i <= PartitionNum; i++) {
            freePartition[i] = FreePartition[i];
        }

        //进程分配
        for (int i = 1; i <= ProcessNum; i++) {
            for (int j = 1; j <= PartitionNum; j++) {
                if (freePartition[j] >= progressCopy[i].ProcessNeed) {
                    freePartition[j] -= progressCopy[i].ProcessNeed;
                    progressCopy[i].FirstPartition = j;
                    break;
                }
            }
        }

        Test(progressCopy);

        Print(progressCopy, freePartition);

        NextAlgorithm();
    }
    //最佳适应分区算法
    //调用BestPartition算法进行调度计算
    void AlgorithmBestPartition() {
        //复制进程
        Progress progressCopy[MaxNumber];
        for (int i = 1; i <= ProcessNum; i++) {
            progressCopy[i] = progress[i];
        }

        //复制空闲分区大小
        int freePartition[MaxNumber];//空闲分区大小
        for (int i = 1; i <= PartitionNum; i++) {
            freePartition[i] = FreePartition[i];
        }

        //进程分配
        for (int i = 1; i <= ProcessNum; i++) {
            for (int j = 1; j <= PartitionNum; j++) {
                if (freePartition[AscendingBubbleSort(freePartition, j)] >= progressCopy[i].ProcessNeed) {
                    progressCopy[i].BestPartition = AscendingBubbleSort(freePartition, j);
                    freePartition[AscendingBubbleSort(freePartition, j)] -= progressCopy[i].ProcessNeed;
                    break;
                }
            }
        }

        Test(progressCopy);

        Print(progressCopy, freePartition);

        NextAlgorithm();


    }

    //辅助算法：升序冒泡排序算法，将分区剩余大小从小到大冒泡排序，返回该index排序的值
    int AscendingBubbleSort(const int freePartition[MaxNumber], int index) {
        //复制空闲分区
        int freePartitionCopy[MaxNumber];//空闲分区大小
        for (int i = 1; i <= PartitionNum; i++) {
            freePartitionCopy[i] = freePartition[i];
        }

        //将分区剩余大小从小到大冒泡排序
        for (int i = 1; i <= PartitionNum; i++) {
            for (int j = 1; j <= PartitionNum - i; j++) {
                if (freePartitionCopy[j] > freePartitionCopy[j + 1]) {
                    int temp = freePartitionCopy[j];
                    freePartitionCopy[j] = freePartitionCopy[j + 1];
                    freePartitionCopy[j + 1] = temp;
                }
            }
        }

        //查找该最短服务时间对应的进程号
        for (int j = 1; j <= PartitionNum; j++) {
            if (freePartition[j] == freePartitionCopy[index]) {
                return j;
            }
        }

        return 0;
    }
    //最坏适应分区算法
    //调用WorstPartition算法进行调度计算
    void AlgorithmWorstPartition() {
        //复制进程
        Progress progressCopy[MaxNumber];
        for (int i = 1; i <= ProcessNum; i++) {
            progressCopy[i] = progress[i];
        }

        //复制空闲分区大小
        int freePartition[MaxNumber];//空闲分区大小
        for (int i = 1; i <= PartitionNum; i++) {
            freePartition[i] = FreePartition[i];
        }

        //进程分配
        for (int i = 1; i <= ProcessNum; i++) {
            for (int j = 1; j <= PartitionNum; j++) {
                if (freePartition[DescendingBubbleSort(freePartition, j)] >= progressCopy[i].ProcessNeed) {
                    progressCopy[i].WorstPartition = DescendingBubbleSort(freePartition, j);
                    freePartition[DescendingBubbleSort(freePartition, j)] -= progressCopy[i].ProcessNeed;
                    break;
                }
            }
        }

        Test(progressCopy);

        Print(progressCopy, freePartition);

        NextAlgorithm();
    }

    //辅助算法：降序冒泡排序算法，将分区剩余大小从大到小冒泡排序，返回该index排序的值
    int DescendingBubbleSort(const int freePartition[MaxNumber], int index) {
        //复制空闲分区
        int freePartitionCopy[MaxNumber];//空闲分区大小
        for (int i = 1; i <= PartitionNum; i++) {
            freePartitionCopy[i] = freePartition[i];
        }

        //将分区剩余大小从大到小冒泡排序
        for (int i = 1; i <= PartitionNum; i++) {
            for (int j = 1; j <= PartitionNum - i; j++) {
                if (freePartitionCopy[j] < freePartitionCopy[j + 1]) {
                    int temp = freePartitionCopy[j];
                    freePartitionCopy[j] = freePartitionCopy[j + 1];
                    freePartitionCopy[j + 1] = temp;
                }
            }
        }

        //查找该最短服务时间对应的进程号
        for (int j = 1; j <= PartitionNum; j++) {
            if (freePartition[j] == freePartitionCopy[index]) {
                return j;
            }
        }

        return 0;
    }

    //验证方法：验证空闲分区是否足够分配
    void Test(Progress pg[MaxNumber]) {
        switch (isAlgorithm) {
            case 1:
                for (int i = 1; i <= ProcessNum; i++) {
                    if (pg[i].FirstPartition < 0) {
                        cout << "空闲分区不足本算法分配" << endl;
                        exit(0);
                    }
                }
                break;
            case 2:
                for (int i = 1; i <= ProcessNum; i++) {
                    if (pg[i].BestPartition < 0) {
                        cout << "空闲分区不足本算法分配" << endl;
                        exit(0);
                    }
                }
                break;
            case 3:
                for (int i = 1; i <= ProcessNum; i++) {
                    if (pg[i].WorstPartition < 0) {
                        cout << "空闲分区不足本算法分配" << endl;
                        exit(0);
                    }
                }
                break;
            
        }
    }

    //询问是否还要进行其余算法
    void NextAlgorithm() {
        cout << endl
             << "请问是否还要进行其余算法，若是，请输入（1-3值）；若否，请输入任意字符（1-首次适应分区算法（FirstPartition），2-最佳适应分区算法（BestPartition），3-最坏适应分区算法（WorstPartition））：";
        cin >> isAlgorithm;
        if (isAlgorithm != 1 && isAlgorithm != 2 && isAlgorithm != 3 ) {
            return;
        }
        IsAlgorithm();
    }
};

int main() {
    PartitionAlgorithm p{};

    p.Input();

    return 0;
}