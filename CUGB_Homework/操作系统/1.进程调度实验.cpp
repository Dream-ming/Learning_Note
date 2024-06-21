#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct job {
    int id;          // 作业的编号
    char name[10];   // 作业的名字
    int starttime;   // 作业到达系统时间
    int needtime;    // 作业运行时间
    int endtime;     // 作业结束时间
    int runtime;     // 作业周转时间
    int restime;     // 剩余时间
    int wattime;     // 等待时间
} jobs[50];

void RR(job *jobs, int n);
void result(job *jobs, int n);
bool cmp1(job &a, job &b) { return a.starttime < b.starttime; }

int main() {

    int n;

    cout << "请输入作业个数：\n";
    cin >> n;

    cout << "请输入每个作业的作业名,到达时间,服务时间:\n";
    for (int i = 1; i <= n; ++i) {
        jobs[i].id = i;
        cin >> jobs[i].name 
            >> jobs[i].starttime 
            >> jobs[i].needtime;
        jobs[i].restime = jobs[i].needtime;
    }

    cout << left << setw(11) << "作业名"
         << left << setw(11) << "到达时间"
         << left << setw(11) << "运行时间"
         << "\n";

    for (int i = 1; i <= n; ++i) {
        cout << left << setw(11) << jobs[i].name
             << left << setw(11) << jobs[i].starttime
             << left << setw(11) << jobs[i].needtime
             << "\n";
    }

    sort(jobs + 1, jobs + n + 1, cmp1);
    RR(jobs, n);
    cout << "RR运行结果:\n";
    result(jobs, n);

    system("pause");
    return 0;
}

void RR(job *jobs, int n) {
    int tim = 0, num = 0, q = 2, r = 1;
    queue<job> q_jobs;
    q_jobs.push(jobs[1]);

    cout << "\n时间片q = " << q << "\n\n";
    while (num < n) {
        job tmp = q_jobs.front();
        q_jobs.pop();
        cout << left << setw(3) << tim 
             << "时刻" << " --> ";
        if (tmp.restime > q) {
            tmp.restime -= q;
            q_jobs.push(tmp);
            tim += q;
        }
        else if (tmp.restime == q){
            tmp.restime -= q;
            tim += q;
            int u = tmp.id;
            jobs[u].endtime = tim;
            jobs[u].runtime = tim - jobs[u].starttime;
            jobs[u].wattime = jobs[u].runtime - jobs[u].needtime;
            ++num;
        } 
        else {
            tim += tmp.restime;
            tmp.restime = 0;
            int u = tmp.id;
            jobs[u].endtime = tim;
            jobs[u].runtime = tim - jobs[u].starttime;
            jobs[u].wattime = jobs[u].runtime - jobs[u].needtime;
            ++num;
        }
        cout << left << setw(3) << tim << 
             "时刻 " << tmp.name << "\n";
        for (int i = r + 1; i <= n; ++i) {
            if (jobs[i].starttime <= tim) {
                q_jobs.push(jobs[i]);
                ++r;
            }
        }
    }
}

void result(job *jobs, int n) {
    int i;
    double averruntime, averwattime;
    int sum_runtime = 0, sum_wattime = 0;
    cout << left << setw(11) << "作业名"
         << left << setw(11) << "到达时间"
         << left << setw(11) << "运行时间"
         << left << setw(11) << "完成时间"
         << left << setw(11) << "等待时间"
         << left << setw(11) << "周转时间"
         << "\n";
    for (int i = 1; i <= n; ++i) {
        cout << left << setw(11) << jobs[i].name
             << left << setw(11) << jobs[i].starttime
             << left << setw(11) << jobs[i].needtime
             << left << setw(11) << jobs[i].endtime
             << left << setw(11) << jobs[i].wattime
             << left << setw(11) << jobs[i].runtime
             << "\n";
        sum_runtime = sum_runtime + jobs[i].runtime;
        sum_wattime = sum_wattime + jobs[i].wattime;
    }
    averruntime = sum_runtime * 1.0 / n;
    averwattime = sum_wattime * 1.0 / n;
    cout << "平均等待时间"
         << fixed << setprecision(3) << averwattime << "\n"
         << "平均周转时间"
         << fixed << setprecision(3) << averruntime << "\n";
}
