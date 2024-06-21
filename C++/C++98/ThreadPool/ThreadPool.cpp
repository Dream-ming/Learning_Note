#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "ThreadPool.h"
using namespace std;

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max)
{
    // 实例化任务队列
    do
    {
        taskQ = new TaskQueue<T>;
        if (taskQ == nullptr)
        {
            cout << "malloc taskQ fail..." << endl;
            break;
        }
        threadIDs = new pthread_t[max];
        if (threadIDs == nullptr)
        {
            cout << "malloc threadIDs fail..." << endl;
            break;
        }
        memset(threadIDs, 0, sizeof(pthread_t) * max);

        minNum = min;
        maxNum = max;
        busyNum = 0;
        liveNum = min;
        exitNum = 0;

        pthread_mutex_init(&mutexPool, NULL);
        pthread_cond_init(&notEmpty, NULL);

        shutdown = false;

        // 创建线程
        pthread_create(&managerID, NULL, manager, this);
        for (int i = 0; i < min; ++i)
        {
            pthread_create(&threadIDs[i], NULL, worker, this);
        }
        return;
    } while (0);

    // 没有return说明构造失败，需要释放资源
    if (threadIDs) delete[]threadIDs;
    if (taskQ) delete taskQ;
}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
    // 关闭线程池
    shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(managerID, NULL);
    // 唤醒阻塞的消费者线程，线程发现 shutdown 就会自杀
    for (int i = 0; i < liveNum; ++i)
    {
        pthread_cond_signal(&notEmpty);
    }

    // 释放堆内存
    if (taskQ)
    {
        delete taskQ;
    }
    if (threadIDs)
    {
        delete[]threadIDs;
    }

    pthread_mutex_destroy(&mutexPool);
    pthread_cond_destroy(&notEmpty);
}

template <typename T>
void ThreadPool<T>::addTask(Task<T> task)
{

    if (shutdown)
    {
        return;
    }

    taskQ->addTask(task);
    pthread_cond_signal(&notEmpty);
}

template <typename T>
int ThreadPool<T>::getBusyNum()
{
    pthread_mutex_lock(&mutexPool);
    int busyNum = this->busyNum;
    pthread_mutex_unlock(&mutexPool);
    return 0;
}

template <typename T>
int ThreadPool<T>::getAliveNum()
{
    pthread_mutex_lock(&mutexPool);
    int aliveNum = this->liveNum;
    pthread_mutex_unlock(&mutexPool);
    return 0;
}

template <typename T>
void* ThreadPool<T>::worker(void* arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    while (true)
    {
        pthread_mutex_lock(&pool->mutexPool);
        // 当前队列是否为空
        while (pool->taskQ->getTaskNumber() == 0 && !pool->shutdown)
        {
            // 阻塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // 判断是否要销毁线程
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
            }
        }

        // 判断线程池是否被关闭了
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        } 

        // 从任务队列中取出一个任务
        Task<T> task = pool->taskQ->takeTask();
        // 解锁
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexPool);

        cout << "thread " << to_string(pthread_self()) << " start working..." << endl;
        task.function(task.arg);
        delete task.arg;
        task.arg = nullptr;

        cout << "thread " << to_string(pthread_self()) << "end working..." << endl;
        
        pthread_mutex_lock(&pool->mutexPool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);
    }
    return nullptr;
}

template <typename T>
void* ThreadPool<T>::manager(void* arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);

    while (!pool->shutdown)
    {
        // 每隔3s检测一次
        sleep(3);

        // 取出线程池中任务个数，活着的线程的个数，忙的线程的个数
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->taskQ->getTaskNumber();
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 添加线程
        // 任务的个数 > 存活的线程个数 && 存活的线程数 < 最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
                pthread_mutex_unlock(&pool->mutexPool);
            }
        }

        // 销毁线程
        // 忙的线程 * 2 < 存活的线程数 && 存活的线程 > 最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER; // 增加销毁数量
            pthread_mutex_unlock(&pool->mutexPool);
            // 唤醒exitNum个被堵塞的线程，线程工作任务会在有exitNum时自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return nullptr;
}

template <typename T>
void ThreadPool<T>::threadExit()
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < maxNum; ++i)
    {
        if (threadIDs[i] == tid)
        {
            threadIDs[i] = 0;
            cout << "threadExit() called, " << to_string(tid) << " exiting..." << endl;
            break;
        }
    }
    pthread_exit(NULL);
}
