#pragma once
#include <queue>
#include <pthread.h>

// 任务结构体
using callback = void(*)(void* arg);

template <typename T>
struct Task;

template <typename T>
struct Task
{
    Task<T>()
    {
        function = nullptr;
        arg = nullptr;
    }
    Task<T>(callback f, void* arg)
    {
        function = f;
        this->arg = (T*)arg;
    }
    callback function = nullptr;
    T* arg;
};

template <typename T>
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task<T> task);
    void addTask(callback f, void* arg);
    // 取出一个任务
    Task<T> takeTask();
    // 获取当前任务的个数
    inline size_t getTaskNumber() {
        return m_taskQ.size();
    }

private:
    pthread_mutex_t m_mutex;
    std::queue<Task<T>> m_taskQ;
};

