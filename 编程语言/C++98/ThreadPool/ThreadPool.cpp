#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include "ThreadPool.h"
using namespace std;

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max)
{
    // ʵ�����������
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

        // �����߳�
        pthread_create(&managerID, NULL, manager, this);
        for (int i = 0; i < min; ++i)
        {
            pthread_create(&threadIDs[i], NULL, worker, this);
        }
        return;
    } while (0);

    // û��return˵������ʧ�ܣ���Ҫ�ͷ���Դ
    if (threadIDs) delete[]threadIDs;
    if (taskQ) delete taskQ;
}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
    // �ر��̳߳�
    shutdown = 1;
    // �������չ������߳�
    pthread_join(managerID, NULL);
    // �����������������̣߳��̷߳��� shutdown �ͻ���ɱ
    for (int i = 0; i < liveNum; ++i)
    {
        pthread_cond_signal(&notEmpty);
    }

    // �ͷŶ��ڴ�
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
        // ��ǰ�����Ƿ�Ϊ��
        while (pool->taskQ->getTaskNumber() == 0 && !pool->shutdown)
        {
            // ���������߳�
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // �ж��Ƿ�Ҫ�����߳�
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

        // �ж��̳߳��Ƿ񱻹ر���
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        } 

        // �����������ȡ��һ������
        Task<T> task = pool->taskQ->takeTask();
        // ����
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
        // ÿ��3s���һ��
        sleep(3);

        // ȡ���̳߳���������������ŵ��̵߳ĸ�����æ���̵߳ĸ���
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->taskQ->getTaskNumber();
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // ����߳�
        // ����ĸ��� > �����̸߳��� && �����߳��� < ����߳���
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

        // �����߳�
        // æ���߳� * 2 < �����߳��� && �����߳� > ��С�߳���
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER; // ������������
            pthread_mutex_unlock(&pool->mutexPool);
            // ����exitNum�����������̣߳��̹߳������������exitNumʱ��ɱ
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
