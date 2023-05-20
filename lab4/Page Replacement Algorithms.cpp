//2205112E39李萍
#include<iostream>
#include<algorithm>
#include<array>
#include <vector>
#include <unordered_set>
#include <unistd.h>
#include <cstdlib>
/*
windows#include <cstdlib>
system("pause");
在linux系统下会出现如题的错误，这是因为linux不认识 system("pause"); 这条语句，改为：
#include <unistd.h>
pause();
*/
using namespace std;

array<int, 17>order = { 1,2,5,7,5,7,1,4,3,5,6,4,3,2,1,5,2 };
//array<int, 20>order = { 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };


class page_replacement {
private:
	//OPT数据结构
	typedef struct opt {
		int next = 99999; //与此相同的下一个数据
		int order = -1; //页面序号
	}opt;
	//FIFO数据结构
	typedef struct fifo {
		int first = -1; //最早到达页号
		int order = -1; //页面序号
	}fifo;
	//LRU数据结构
	typedef struct lru {
		int recent = 0x80;  //时间戳
		int order = -1;
	}lru;
	//int data;
	typedef struct clock {
        int order = -1;// 初始化页面号为-1，表示物理块为空
        bool reference = false;// 初始化引用位为false
    } clock;
	
public:
	//构造函数
	page_replacement() {
		//cout << "Object is being created!" << endl;
	}//page_replacement
	//析构函数
	~page_replacement() {
		//cout << "\nObject is being deleted!" << endl;
	}//~page_replacement
	void OPT();
	void FIFO();
	void LRU();
	void Clock();
	void format(int len);
	template<typename T1, typename T2>
	void printResult(int len, int ans, int cnt, T1 &rede, T1 &elim, T2 &block_tencent);
};



//格式调整
void page_replacement::format(int len) {
	for (int i = 0; i < len; i++)
		cout << "- - ";
	cout << endl;
}//format

//page函数模板
template<class T1, class T2>
void page_replacement::printResult(int len, int ans, int cnt, T1 &rede, T1 &elim, T2 &block_content) {
	cout << "物理块页面内容：" << endl;
	format(len);
	for (int i = 0; i < len; i++) {
		if (i == 0)
			cout << "  " << order[i] << " ";
		else
			cout << ": " << order[i] << " ";
	}//for
	cout << endl;
	format(len);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < block_content.size(); j++) {
			if (block_content[j][i].order == -1) {
				cout << "    ";
				continue;
			}//if
			if (j - 1 >= 0 && block_content[j - 1][i].order == -1 || j == 0)
				cout << "  " << block_content[j][i].order << " ";
			else
				cout << ": " << block_content[j][i].order << " ";
		}//for
		cout << endl;
	}//for
	format(len);
	//迭代器
	cout << "换出页面序列：        ";
	for (auto iter = elim.begin(); iter != elim.end(); iter++)
		cout << iter->order << " ";
	cout << "\n换入页面序列：";
	for (auto iter = rede.begin(); iter != rede.end(); iter++)
		cout << iter->order << " ";
	float result = (float)(ans + cnt) / len;
	cout << "\n缺页次数：" << cnt << " | 页面置换次数：" << ans;
	cout << endl << "缺页率：" << result << endl;
}//printResult

//最佳置换算法
void page_replacement::OPT() {
	int len = order.size();
	int cnt = 0;  //缺页次数
	int ans = 0;  //页面置换
	vector<opt>opt_arr(len);
	vector<opt>rede; //换入
	vector<opt>elim; //换出
	array<opt, 4>block_memory; //物理块
	vector<array<opt, 4>>block_content;

	//初始化opt_arr
	for (int i = 0; i < len; i++) {
		opt_arr[i].order = order[i];
		int index = 0;
		for (int j = i+1; j < len; j++) {
			index++;
			if (opt_arr[i].order == order[j]) {
				opt_arr[i].next = index;
				break;
			}//if
		}//for
		cout << index << " ";
	}//for
	cout << endl;
	//页面调用
	for (int i = 0; i < len; i++) {
		//物理块空间充足
		if (i<4) {
			block_memory[i] = opt_arr[i];
			rede.push_back(opt_arr[i]);
			block_content.push_back(block_memory);
			ans++;
			for (int j = 0; j < i; j++)
				block_memory[j].next -= 1;
			continue;
		}//if
		//物理块空间不足,需要页面置换
		else {
			//如果物理块中存在该页面，直接引用即可
			bool flag = true;
			int index = 0;
			int future = -1;
			int tmp = future;
			for (int j = 0; j < 4; j++) {
				if (block_memory[j].order == opt_arr[i].order) {
					block_memory[j].next = opt_arr[i].next;
					flag = false;
					break;
				}//if
				else {
					tmp = max(tmp, block_memory[j].next);
					if (tmp > future) {
						future = tmp;
						index = j;
					}//if
				}//else
			}//for
			if (flag) {
				//置换页面
				elim.push_back(block_memory[index]);//换出页面
				block_memory[index] = opt_arr[i];
				rede.push_back(opt_arr[i]);//换入页面
				cnt++;  //缺页次数加1
			}//if
			for (int j = 0; j < 4; j++) {
				block_memory[j].next -= 1;
			}//for
			block_content.push_back(block_memory);
		}//else
	}//for
	//cout << "\n\nOPT最佳置换算法\n";
	cout<<"---------------------OPT最佳置换算法的结果为：---------------------"<<endl;
	printResult(len, ans, cnt, rede, elim, block_content);
}//OPT

//先来先服务置换算法
void page_replacement::FIFO() {
	int len = order.size();
	int ans = 0;
	int cnt = 0;
	array<fifo, 4>block;  //物理块容量为4
	vector<array<fifo, 4>>block_content;
	vector<fifo>fifo_f(len);
	vector<fifo>elim, rede;

	//初始化
	for (int i = 0; i < len; i++) {
		fifo_f[i].order = order[i];
	}//for
	//页面调用
	for (int i = 0; i < order.size(); i++) {
		//物理块空间充足
		if (i < 4) {
			block[i].order = order[i];
			block[i].first += 1;
			rede.push_back(block[i]);
			block_content.push_back(block);
			for (int j = 0; j < i; j++) {
				block[j].first += 1;
			}//for
			ans++;
			continue;
		}//if
		//物理块空间不足
		else {
			bool flag = true;
			int index = 0;
			int max_fifo = -1;
			int tmp = max_fifo;
			//计算待换出的页面
			for (int j = 0; j < 4; j++) {
				if (block[j].order == fifo_f[i].order) {
					flag = false;
					break;
				}//if
				else {
					tmp = max(tmp, block[j].first);
					if (tmp > max_fifo) {
						max_fifo = tmp;
						index = j;
					}//if
				}//else
			}//for
			//置换页面
			if (flag) {
				elim.push_back(block[index]);
				block[index] = fifo_f[i];
				rede.push_back(fifo_f[i]);
				cnt++;
				for (int j = 0; j < 4; j++) {
					if (j == index)continue;
					block[j].first += 1;
				}//for
			}//if
			block_content.push_back(block);
		}//else
	}//for
	//输出置换结果
	//cout << "\n\nFIFO先来先服务置换算法\n";
	cout<<"---------------------FIFO先来先服务置换算法的结果为：---------------------"<<endl;
	printResult(len, ans, cnt, rede, elim, block_content);
}//FIFO

//最近最久未使用置换算法
void page_replacement::LRU() {
	int len = order.size();
	int ans = 0, cnt = 0;
	array<lru, 4>block;  //物理块容量为4
	vector<array<lru, 4>>block_content;
	vector<lru>lru_r(len);
	vector<lru>elim, rede;

	//初始化LRU
	for (int i = 0; i < len; i++) {
		lru_r[i].order = order[i];
	}//for
	//页面调用
	for (int i = 0; i < len; i++) {
		//物理块空间充足
		if (i < 4) {
			block[i] = lru_r[i];
			rede.push_back(block[i]);
			block_content.push_back(block);
			ans++;
			for (int j = 0; j < i; j++)
				block[i].recent >>= 1;
			for (int j = 0; j <= i; j++)
				cout << block[j].recent << " ";
			cout << endl;
		}//if
		//物理块空间不足
		else {
			bool flag = true;
			int last_no_used = 99999;
			int tmp = last_no_used;
			int index = 0;
			for (int j = 0; j < 4; j++)
				block[j].recent >>= 1;
			for (int j = 0; j < 4; j++) {
				if (block[j].order == lru_r[i].order) {
					block[j].recent |= 0x80;
					flag = false;
					break;
				}//if
				else {
					tmp = min(tmp, block[j].recent);
					if (tmp < last_no_used) {
						last_no_used = tmp;
						index = j;
					}//if
				}//else
			}//for
			if (flag) {
				elim.push_back(block[index]);
				rede.push_back(lru_r[i]);
				block[index] = lru_r[i];
				block[index].recent |= 0x80;
				cnt++;
			}//flag
			block_content.push_back(block);
		}//else
	}//for
	//输出置换结果
	//cout << "\n\nLRU最近最久未使用置换算法\n";
	cout<<"---------------------LRU最近最久未使用置换算法的结果为：---------------------"<<endl;
	printResult(len, ans, cnt, rede, elim, block_content);
}//LRU
//clock算法
void page_replacement::Clock() {
    int len = order.size();
    int cnt = 0; // 缺页次数
    int ans = 0; // 页面置换次数
    vector<clock> clock_arr(len); // 用于记录页面的数组
    vector<clock> rede; // 换入页面序列
    vector<clock> elim; // 换出页面序列;
    array<clock, 4> block_memory;
    vector<array<clock, 4>> block_content;// 物理块的页面内容记录

    for (int i = 0; i < len; i++) {
        clock_arr[i].order = order[i];// 记录页面号
        clock_arr[i].reference = false; // 初始化引用位为false
    }

    int hand = 0; // Clock指针位置，指向下一个要检查的物理块
    int blockSize = 4; // 物理块大小

    for (int i = 0; i < len; i++) {
         // 检查页面是否在物理块中
        bool found = false;
        for (int j = 0; j < blockSize; j++) {
            if (block_memory[j].order == order[i]) {
                clock_arr[i].reference = true;// 页面已经在物理块中，设置引用位为true
                found = true;
                break;
            }
        }

        if (!found) { // 页面不在物理块中，发生页面缺失
            cnt++; // 缺页次数加一

            // 找到下一个未被引用的物理块位置
            while (block_memory[hand].reference) {
                block_memory[hand].reference = false; // 将已被引用的页面引用位重置为false
                hand = (hand + 1) % blockSize; // Clock指针向后移动一位
            }

            if (block_memory[hand].order != -1) {
                elim.push_back(block_memory[hand]); // 将被替换出的页面加入换出页面序列
                ans++; // 页面置换次数加一
            }

            rede.push_back(clock_arr[i]); // 将新页面加入换入页面序列
            block_memory[hand] = clock_arr[i]; // 将页面放入物理块中
            hand = (hand + 1) % blockSize; // Clock指针向后移动一位

            // 记录物理块内容
            array<clock, 4> block; // 创建一个新的物理块数组，用于记录物理块的当前内容
            for (int j = 0; j < blockSize; j++) {
                block[j] = block_memory[j]; // 将物理块中的页面复制到新的物理块数组中
            }
            block_content.push_back(block); // 将物理块内容记录添加到记录数组中
        }
    }
	cout<<"---------------------clock算法的结果为：---------------------"<<endl;
    printResult(len, ans, cnt, rede, elim, block_content);
}
// 计算进程的工作集算法
std::vector<int> CalculateWorkingSet(const std::vector<int>& memoryAccesses, int windowSize) {
    std::vector<int> workingSet;
    std::unordered_set<int> pageTable;

    for (int i = 0; i < memoryAccesses.size(); i++) {
        int page = memoryAccesses[i];

        // 添加页面到工作集
        pageTable.insert(page);

        // 如果窗口大小超过限制，则移除最早访问的页面
        if (pageTable.size() > windowSize) {
            int oldestPage = memoryAccesses[i - windowSize];
            pageTable.erase(oldestPage);
        }

        // 将当前工作集大小加入结果向量
        workingSet.push_back(pageTable.size());
    }

    return workingSet;
}
// 工作集置换算法
void working_set_replacement(const vector<int>& page_references, int window_size) {
    unordered_set<int> working_set; // 工作集
    int page_faults = 0; // 缺页次数

    for (int i = 0; i < page_references.size(); i++) {
        int page = page_references[i];

        // 检查页面是否在工作集中
        if (working_set.find(page) == working_set.end()) {
            cout << "发生页面错误，页面号：" << page << endl;
            page_faults++;

            // 添加页面到工作集
            working_set.insert(page);

            // 如果工作集超过窗口尺寸，则进行页面置换
            if (working_set.size() > window_size) {
                int oldest_page = *min_element(working_set.begin(), working_set.end());
                working_set.erase(oldest_page);
                cout << "从工作集中淘汰页面：" << oldest_page << endl;
            }
        }

        cout << "当前工作集：";
        for (int page : working_set) {
            cout << page << " ";
        }
        cout << endl;
    }

    cout << "总页面错误次数：" << page_faults << endl;
}

vector<int> get_working_set(vector<int>& page_refs, int window_size) {
    vector<int> working_set;
    vector<int> recent_refs(window_size, -1); // 初始化为-1
    int current_time = 0;

    for (auto ref : page_refs) {
        ++current_time;
        auto it = find(recent_refs.begin(), recent_refs.end(), ref);
        if (it != recent_refs.end()) {
            // 如果引用在窗口中，更新时间戳
            *it = ref;
        } else {
            // 如果引用不在窗口中，将其加入窗口中
            int oldest_time = current_time;
            int oldest_ref = -1;
            for (auto r : recent_refs) {
                auto found = find(page_refs.begin(), page_refs.end(), r);
                if (found == page_refs.end()) {
                    // 如果引用不再页面引用序列中，不考虑其时间戳
                    continue;
                }
                int time_diff = current_time - (found - page_refs.begin());
                if (time_diff > window_size) {
                    // 如果超出窗口尺寸，认为其过期
                    oldest_ref = r;
                    break;
                } else if (time_diff < oldest_time) {
                    // 如果不超出窗口尺寸，取最久未使用的引用
                    oldest_time = time_diff;
                    oldest_ref = r;
                }
            }
            *find(recent_refs.begin(), recent_refs.end(), oldest_ref) = ref;
        }
        working_set = recent_refs;
        working_set.erase(remove(working_set.begin(), working_set.end(), -1), working_set.end());
    }

    return working_set;
}

int main() {
	page_replacement page_r;
	page_r.OPT();
	page_r.FIFO();
	page_r.LRU();
	page_r.Clock();
	//——————————
	vector<int> page_references(17);
	vector<int> memoryAccesses(17);
	for(int i=0;i<17;i++){
		memoryAccesses[i]=order[i];
		page_references[i]=order[i];
	}

    //int window_size = 3;

    // std::vector<int> workingSet = CalculateWorkingSet(memoryAccesses, window_size);

    // std::cout << "工作集大小: ";
    // for (int size : workingSet) {
    //     std::cout << size << " ";
    // }
    // std::cout << std::endl;

    int window_size1 = 3;
	cout<<"---------------------工作集窗口尺寸3的结果为：---------------------"<<endl;
    cout << "工作集窗口尺寸：" << window_size1 << endl;
    working_set_replacement(page_references, window_size1);
    cout << endl;

    int window_size2 = 4;
	cout<<"---------------------工作集窗口尺寸4的结果为：---------------------"<<endl;
    cout << "工作集窗口尺寸：" << window_size2 << endl;
    working_set_replacement(page_references, window_size2);

	pause();
	return 0;
}
