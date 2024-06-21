#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#define MAX 10
// 全局变量
int number;

pthread_mutex_t mutex;

// 线程处理函数
void* funcA_num(void* arg) {
    for(int i=0; i<MAX; ++i) {
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur++;
        usleep(10);
        number = cur;
        printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* funcB_num(void* arg) {
    for(int i=0; i<MAX; ++i) {
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur++;
        number = cur;
        printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
        pthread_mutex_unlock(&mutex);
        usleep(5);
    }

    return NULL;
}

int main(int argc, const char* argv[]) {
    pthread_t p1, p2;

    // 互斥锁初始化
    pthread_mutex_init(&mutex, NULL);

    // 创建两个子线程
    pthread_create(&p1, NULL, funcA_num, NULL);
    pthread_create(&p2, NULL, funcB_num, NULL);

    // 阻塞，资源回收
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    // 使用结束，销毁互斥锁，释放资源
    pthread_mutex_destroy(&mutex);

    return 0;
}
/*
运行结果：
Thread A, id = 132221105075776, number = 1
Thread A, id = 132221105075776, number = 2
Thread A, id = 132221105075776, number = 3
Thread A, id = 132221105075776, number = 4
Thread A, id = 132221105075776, number = 5
Thread A, id = 132221105075776, number = 6
Thread A, id = 132221105075776, number = 7
Thread A, id = 132221105075776, number = 8
Thread A, id = 132221105075776, number = 9
Thread A, id = 132221105075776, number = 10
Thread B, id = 132221096683072, number = 11
Thread B, id = 132221096683072, number = 12
Thread B, id = 132221096683072, number = 13
Thread B, id = 132221096683072, number = 14
Thread B, id = 132221096683072, number = 15
Thread B, id = 132221096683072, number = 16
Thread B, id = 132221096683072, number = 17
Thread B, id = 132221096683072, number = 18
Thread B, id = 132221096683072, number = 19
Thread B, id = 132221096683072, number = 20
*/
