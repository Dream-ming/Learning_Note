#pragma once
#include "TaskQueue.h"

template <typename T>
class ThreadPool
{
public:
    // �����̳߳�
    ThreadPool(int min, int max);

    // �����̳߳�
    ~ThreadPool();

    // ���̳߳��������
    void addTask(Task<T> task);

    // ��ȡ�̳߳��й����е��̸߳���
    int getBusyNum();

    // ��ȡ�̳߳��л��ߵ��̸߳���
    int getAliveNum();

private:
    // �������߳�������
    static void* worker(void* arg);

    // �������߳�������
    static void* manager(void* arg);

    // �����߳��Ƴ�
    void threadExit();

private:
    // �������
    TaskQueue<T>* taskQ;

    pthread_t managerID;    // �������߳�ID
    pthread_t* threadIDs;    // �������߳�ID

    int minNum;     // ��С�߳���
    int maxNum;     // ����߳���
    int busyNum;    // æ���߳���
    int liveNum;    // �����߳���
    int exitNum;    // Ҫ���ٵ��̸߳���

    pthread_mutex_t mutexPool;      // �������̳߳�
    pthread_cond_t notEmpty;        // ��������ǲ��ǿ���
    static const int NUMBER = 2;    // �������߳�һ�������������߳�

    bool shutdown;       // �ǲ���Ҫ�����̳߳أ�����Ϊ1
};

