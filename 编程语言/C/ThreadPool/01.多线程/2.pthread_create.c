#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* callback(void* arg) {
    // printf("%ld\n", pthread_self()); 输出当前线程的 id
    for (int i = 0; i < 5; ++i) {
        printf("子线程:i = %d\n", i);
    }
    printf("%ld\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    // 创建新的线程，四个参数分别为：一个线程变量，线程属性，任务函数，给任务函数传的参数
    for (int i = 0; i < 5; ++i) {
        printf("主线程: i = %d\n", i);
    }
    printf("主线程: %ld\n", pthread_self());
    return 0;
}

/*
主线程结束过快，会回收子线程，导致子线程没有执行完全
可以在 return 0前，sleep(3)，让子线程充分执行
*/
