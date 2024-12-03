#pragma once
#include "TaskQueue.h"

template <typename T>
class ThreadPool
{
public:
    // 创建线程池
    ThreadPool(int min, int max);

    // 销毁线程池
    ~ThreadPool();

    // 给线程池添加任务
    void addTask(Task<T> task);

    // 获取线程池中工作中的线程个数
    int getBusyNum();

    // 获取线程池中或者的线程个数
    int getAliveNum();

private:
    // 工作的线程任务函数
    static void* worker(void* arg);

    // 管理者线程任务函数
    static void* manager(void* arg);

    // 单个线程推出
    void threadExit();

private:
    // 任务队列
    TaskQueue<T>* taskQ;

    pthread_t managerID;    // 管理者线程ID
    pthread_t* threadIDs;    // 工作的线程ID

    int minNum;     // 最小线程数
    int maxNum;     // 最大线程数
    int busyNum;    // 忙的线程数
    int liveNum;    // 存活的线程数
    int exitNum;    // 要销毁的线程个数

    pthread_mutex_t mutexPool;      // 锁整个线程池
    pthread_cond_t notEmpty;        // 任务队列是不是空了
    static const int NUMBER = 2;    // 管理者线程一次最多添加两个线程

    bool shutdown;       // 是不是要销毁线程池，销毁为1
};

