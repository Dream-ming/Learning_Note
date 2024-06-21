#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

// 7 0 1 2 0 3 0 4 2 3 0 3

int findLRU(int tim[], int f) {
	int mi = tim[0], res = 0;
	for (int i = 1; i < f; ++i) {
		if (tim[i] < mi) {
			res = i;
			mi = tim[i];
		}
	}
	return res; // 找时间戳最小的替换
}

void LRU(int f) {
	int virtualMemory[f], lastVisTime[f];
	memset(lastVisTime, 0, sizeof(lastVisTime));
	memset(virtualMemory, -1, sizeof(virtualMemory));
	cout << "开始LRU算法: " << endl << endl;
	int x, tim = 0, hitnum = 0, missnum = 0;

	while (1) {
		++tim;
		cout << "请输入下一个被访问的页面(若无, 输入-1)" << endl;
		cin >> x;
		if (x == -1) break;
		for (int i = 0; i < f; ++i) {
			if (virtualMemory[i] == x) {
				++hitnum;
				cout << "hit!" << endl;
				lastVisTime[i] = tim;
				break;
			}
			if (i == f - 1) {
				++missnum;
				cout << "miss!" << endl;
				int tmp = findLRU(lastVisTime, f);
				virtualMemory[tmp] = x;
				lastVisTime[tmp] = tim;
			}
		}
		cout << "当前虚拟内存器存储的页面编号" << endl;
		for (int i = 0; i < f; ++i) {
			cout << virtualMemory[i] << " ";
		}
		cout << endl << endl;
	}

	cout << endl << "LRU算法结束" << endl
		  << "LRU算法的缺页次数: " << missnum << endl
		  << "LRU算法的缺页率: " << missnum * 1.0 / (missnum + hitnum) * 100 << "%"
		  << endl << endl;
}

void FIFO(int f) {
	int l = 0, visnum = 0, hitnum = 0, missnum = 0, x;
	int virtualMemory[f];
	memset(virtualMemory, -1, sizeof(virtualMemory));
	cout << "开始FIFO算法: " << endl << endl;

	while (1) {
		cout << "请输入下一个被访问的页面(若无, 输入-1)" << endl;
		cin >> x;
		if (x == -1) break;
		for (int i = 0; i < f; ++i) {
			if (virtualMemory[i] == x) {
				++hitnum;
				cout << "hit!" << endl;
				break;
			}
			if (i == f - 1) {
				++missnum;
				cout << "miss!" << endl;
				if (visnum < f) {
					virtualMemory[visnum++] = x;
				}
				else {
					virtualMemory[l] = x;
					l = (l + 1) % f;
				}
			}
		}
		cout << "当前虚拟内存器存储的页面编号" << endl;
		for (int i = 0; i < f; ++i) {
			cout << virtualMemory[i] << " ";
		}
		cout << endl << endl;
	}

	cout << endl << "FIFO算法结束" << endl
		  << "FIFO算法的缺页次数: " << missnum << endl
		  << "FIFO算法的缺页率: " << missnum * 1.0 / (missnum + hitnum) * 100 << "%"
		  << endl << endl;
}

int main() {

	int f;
	cout << "请输入虚拟存储器大小: " << endl;
	cin >> f;

	LRU(f);

	FIFO(f);

	return 0;
}
