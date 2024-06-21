#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

const int inf = 0x3f3f3f3f;

void FCFS(vector<int> a, int &n) {
    cout << endl << "使用FCFS算法调度结果" << endl << endl;

    int res1 = 0, res2 = 0;
    cout << "访问顺序: ";
    for (int i = 1; i <= n; ++i) {
        cout << a[i] << " \n"[i == n];
        res1 += abs(a[i] - a[i - 1]) / 8;
        int t1 = min(a[i] % 8, a[i - 1] % 8);
        int t2 = max(a[i] % 8, a[i - 1] % 8);
        res2 += min(t2 - t1, t1 + 8 - t2);
    }

    cout << endl
         << "寻道时间: " << res1 << endl
         << "旋转延迟: " << res2 << endl;
}

void SSTF(vector<int> a, int &n) {
    cout << endl << endl << "使用SSTF算法调度结果" << endl << endl;

    int res1 = 0, res2 = 0, lat = 5;
    cout << "访问顺序: ";
    for (int i = 1; i <= n; ++i) {
        int id = 0, num = inf;
        for (int j = 1; j <= n; ++j) {
            int tmp = abs(a[j] - lat);
            if (tmp <= num)
                id = j, num = tmp;
        }
        res1 += abs(a[id] - lat) / 8;
        int t1 = min(a[id] % 8, lat % 8);
        int t2 = max(a[id] % 8, lat % 8);
        res2 += min(t2 - t1, t1 + 8 - t2);
        cout << a[id] << " \n"[i == n];
        lat = a[id];
        a[id] = inf;
    }

    cout << endl
         << "寻道时间: " << res1 << endl
         << "旋转延迟: " << res2 << endl;
}

void SCAN(vector<int> a, int &n) {
    cout << endl << endl << "使用SCAN算法调度结果" << endl << endl;

    sort(a.begin() + 1, a.begin() + n + 1);
    int p = lower_bound(a.begin() + 1, a.begin() + n + 1, 5) - a.begin();

    int res1 = 0, res2 = 0;
    cout << "访问顺序: ";

    for (int i = p; i <= n; ++i) {
        int t1, t2;
        if (i == p)
            t1 = 5, t2 = a[i];
        else
            t1 = a[i - 1], t2 = a[i];
        res1 += (t2 - t1) / 8;
        t1 %= 8;
        t2 %= 8;
        if (t1 > t2)
            swap(t1, t2);
        res2 += min(t2 - t1, t1 + 8 - t2);
        cout << a[i] << " ";
    }

    for (int i = p - 1; i >= 1; --i) {
        int t1, t2;
        if (i == p - 1)
            t1 = a[i], t2 = a[n];
        else
            t1 = a[i], t2 = a[i + 1];
        res1 += (t2 - t1) / 8;
        t1 %= 8;
        t2 %= 8;
        if (t1 > t2)
            swap(t1, t2);
        res2 += min(t2 - t1, t1 + 8 - t2);
        cout << a[i] << " \n"[i == 1];
    }

    cout << endl
         << "寻道时间: " << res1 << endl
         << "旋转延迟: " << res2 << endl;
}

void CSCAN(vector<int> a, int &n) {
    cout << endl << endl << "使用CSCAN算法调度结果" << endl << endl;

    sort(a.begin() + 1, a.begin() + n + 1);
    int  p = lower_bound(a.begin() + 1, a.begin() + n + 1, 5) - a.begin();

    int res1 = 0, res2 = 0;
    cout << "访问顺序：";

    for (int i = p; i <= n; ++i) {
        int t1, t2;
        if (i == p)
            t1 = 5, t2 = a[i];
        else
            t1 = a[i - 1], t2 = a[i];
        res1 += (t2 - t1) / 8;
        t1 %= 8;
        t2 %= 8;
        if (t1 > t2)
            swap(t1, t2);
        res2 += min(t2 - t1, t1 + 8 - t2);
        cout << a[i] << " ";
    }

    for (int i = 1; i < p; ++i) {
        int t1, t2;
        if (i == 1)
            t1 = a[i], t2 = a[n];
        else
            t1 = a[i], t2 = a[i + 1];
        res1 += (t2 - t1) / 8;
        t1 %= 8;
        t2 %= 8;
        if (t1 > t2)
            swap(t1, t2);
        res2 += min(t2 - t1, t1 + 8 - t2);
        cout << a[i] << " \n"[i == p - 1];
    }

    cout << endl
         << "寻道时间: " << res1 << endl
         << "旋转延迟: " << res2 << endl;
}

int main() {

    srand(time(nullptr));

    cout << "规定: " << endl
         << "每个扇区包含10个磁道号, 一共8个扇区" << endl
         << "磁盘旋转一个扇区为一个单位旋转延迟时间" << endl
         << "磁头移动一个磁道为一个单位寻道时间" << endl
         << "初始时磁头在5号磁道" << endl
         << endl;

    int n = 10;
    cout << "服务的个数"
         << " " << n << endl
         << "每一个服务需要访问的磁道号: ";

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        while (a[i] == 0)
            a[i] = rand() % 89;
        cout << a[i] << " ";
    }

    cout << endl << endl;
    a[0] = 5;

    FCFS(a, n);

    SSTF(a, n);

    SCAN(a, n);

    CSCAN(a, n);

    cout << "\n\n";
    system("pause");

    return 0;
}
