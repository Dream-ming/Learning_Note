#include <cmath>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <queue>
#include <map>
#include <cmath>
using namespace std;

class Mem { // 内存
public:
	int ID;       // 分区的ID
	int size;     // 分区的大小
	int address;  // 分区的开始地址
	int state;    // 0 表示未使用， 1表示使用
	int workID;
	Mem* next;
	Mem(): next(NULL) {}
};

class PCB { // 进程
public:
	int ID;
	int size;
	int address;
	int runState;
	PCB* next;
	PCB(): next(NULL) {}
};

PCB* getPCB(PCB* pcbQ, int pcbId);            // 查询某一个PCB
void showMemInfo(Mem* memReady, PCB* pcbQ);   // 输出内存的信息
void showPCBInfo(PCB* pcbQ);                  // 输出PCB的信息

int getMemLength(Mem* memReady);  // 获取内存块的长度
Mem* inputMem(Mem* memReady);     // 输入固定分区的内存块
Mem* sortMem(Mem* memReady);      // 对内存进行排序

PCB* inputPCB(PCB* pcbQ);                                  //  输入PCB块的信息
Mem* allocateMem(Mem*& memReady, int pcbId, int pcbSize);  // 单个内存的分配
Mem* allocatePCB(Mem*& memReady, PCB*& pcbQ);              // 对所有PCB分配内存

bool checkMemEmpty(Mem* memReady);  // 检测内存是否全部释放
bool checkPCBEmpty(PCB* pcbQ);      // 检测PCB链表是否都在内存中执行

Mem* recoverMem(Mem* memReady, PCB* pcbQ);   // 回收内存

int main() {
	cout << "固定分区,最佳适应算法" << endl;

	Mem* memReady = NULL;

	Mem* mem = inputMem(memReady);

	Mem* sortmem = sortMem(mem);

	PCB* pcbQ = NULL;

	PCB* input_pcb = inputPCB(pcbQ);

	showPCBInfo(input_pcb);

	allocatePCB(sortmem, input_pcb);

	showPCBInfo(input_pcb);

	recoverMem(sortmem, input_pcb);

	return 0;
}

PCB* getPCB(PCB* pcbQ, int pcbId) {
    PCB* p = pcbQ;
    while (p != NULL) {
        if (p->ID == pcbId) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int getMemLength(Mem* memReady) {
	int cnt = 0;
	Mem* p = memReady;
	while (p != NULL) {
		++cnt;
		p = p->next;
	}
	return cnt;
}

void showMemInfo(Mem* memReady, PCB* pcbQ) {
	Mem* p = memReady;
	cout << endl;

	while (p != NULL) {
		cout << "MEM->ID: " << p->ID << ", " 
             << "大小: " << p->size << "B, " 
             << "分配状态：" << p->state << ", "
             << "开始地址：" << p->address << "B";
		if (p->workID != 0) {
			cout << ", 已分配的作业ID: " << p->workID << endl;
            PCB* t = getPCB(pcbQ, p->workID);
            if (t->size == p->size) {
                cout << "MEM " << p->ID << ", 无未使用地址, 无碎片" << endl;
            }
            else {
                cout << "MEM " << p->ID << ", "
                     << "未使用地址: " << p->address + t->size << " - " << p->address + p->size - 1 << ", "
                     << "属于内部碎片" << endl;
            }
		}
		else {
			cout << ", 未分配任何作业" << endl;
            cout << "MEM " << p->ID << ", " 
                 << "未被使用地址: " << p->address << " - " << p->address + p->size - 1 << ", "
                 << "属于外部碎片" << endl;
		}
		p = p->next;
	}
	cout << endl;
}

void showPCBInfo(PCB* pcbQ) {
	PCB* p = pcbQ;
	cout << endl;

	while (p != NULL) {
		cout << "PCB->ID: " << p->ID << ", "
             << "PCB运行状态: " << p->runState << ", "
             << "PCB大小: " << p->size << "B, " 
             << "PCB运行地址: " << p->address << endl;
		p = p->next;
	}

    cout << endl;
}

Mem* inputMem(Mem* memReady) {
	int nowAddress = 0;
	Mem* last = NULL;

	int num;
	cout << "输入内存块的个数: " << endl;
	cin >> num;

	for (int i = 0; i < num; ++i) {
		cout << "第" << i << "号" << "内存, 输入内存的大小(B)" << endl;
		int size;
		cin >> size;

		Mem* tmp = new Mem();
		tmp->ID = i;
		tmp->size = size;
		tmp->state = 0;
		tmp->address = nowAddress;

		if (memReady == NULL) {
			memReady = tmp;
			last = memReady;
		}
		else {
			last->next = tmp;
			last = tmp;
		}

		nowAddress += size;
	}
	showMemInfo(memReady, NULL);
	return memReady;
}

PCB* inputPCB(PCB* pcbQ) {
	cout << "输入作业的个数" << endl;

	int num, siz;
	cin >> num;
	PCB* last = NULL;

	for (int i = 1; i <= num; ++i) {
        cout << "第" << i << "号作业, 输入作业大小(B)" << endl;
		cin >> siz;
		PCB* tmp = new PCB();
		tmp->ID = i;
		tmp->size = siz;
		if (pcbQ == NULL) {
			pcbQ = tmp;
			last = pcbQ;
		}
		else {
			last->next = tmp;
			last = tmp;
		}
	}

	return pcbQ;
}

Mem* sortMem(Mem* memReady) {
	Mem* p1 = memReady;

	if (p1 == NULL || p1->next == NULL) {
		return p1;
	}

    while (p1->next != NULL) {
        Mem* p2 = p1->next;
        while (p2 != NULL) {
            if (p1->size > p2->size) {
                int ID = p1->ID;
                int size = p1->size;
                int address = p1->address;
                int workID = p1->workID;
                int state = p1->state;

                p1->ID = p2->ID;
                p1->size = p2->size;
                p1->address = p2->address;
                p1->workID = p2->workID;
                p1->state = p2->state;

                p2->ID = ID;
                p2->size = size;
                p2->address = address;
                p2->workID = workID;
                p2->state = state;

            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }
	
	return memReady;
}

Mem* allocateMem(Mem*& memReady, int pcbId, int pcbSize) {
	Mem* p = memReady;
	while (p != NULL) {
		if (p->size >= pcbSize) {
			if (p->state == 0) {
				p->state = 1;
				p->workID = pcbId;
				return p;
			}
		}
		p = p->next;
	}
	return NULL;
}

Mem* allocatePCB(Mem*& memReady, PCB*& pcbQ) {
	PCB* p = pcbQ;
    cout << endl;

	while (p != NULL) {
		if (p->runState == 0) {
			Mem* allocated_mem = allocateMem(memReady, p->ID, p->size);
			if (allocated_mem != NULL) {
				cout << "分配成功: PCB_ID: " << p->ID << " 分配的内存块ID: " << allocated_mem->ID << endl;
				p->runState = 1;
				p->address = allocated_mem->address;
			}
            else {
                cout << "分配失败: PCB_ID: " << p->ID << ", 请释放内存" << endl;
            }
		}
		p = p->next;
	}
	return memReady;
}

bool checkMemEmpty(Mem* memReady) {
	Mem* p = memReady;
	while (p != NULL) {
		if (p->state == 1) {
			return false;
		}
		p = p->next;
	}
	return true;
}

bool checkPCBEmpty(PCB* pcbQ) {
	PCB* p = pcbQ;
	while (p != NULL) {
		if (p->runState == 0) {
			return false;
		}
		p = p->next;
	}
	return true;
}

Mem* recoverMem(Mem* memReady, PCB* pcbQ) {
	int num;
	int memLength = getMemLength(memReady);

	while (1) {
		cout << endl;

		showMemInfo(memReady, pcbQ);

		cout << "输入要释放的内存块号" << endl;
		int memBlock;
		cin >> memBlock;

		Mem* p = memReady;
		while (p != NULL) {
			if (p->ID == memBlock) {
				if (p->state == 1) {
					p->state = 0;
					p->workID = 0;
					cout << "内存释放成功" << endl;
					allocatePCB(memReady, pcbQ);
					showMemInfo(memReady, pcbQ);
					showPCBInfo(pcbQ);
				}
			}
			p = p->next;
		}

		if (checkPCBEmpty(pcbQ) && checkMemEmpty(memReady)) {
			cout << "作业已经调度完, 并且内存已释放完" << endl;
			showPCBInfo(pcbQ);
			break;
		}

		char ch;
		cout << "是否继续释放内存 Y -> Yes, N -> No" << endl;
		cin >> ch;
		if (ch == 'N') break;
	}

	cout << endl << "内存状态： " << endl;
	showMemInfo(memReady, pcbQ);
	return memReady;
}
