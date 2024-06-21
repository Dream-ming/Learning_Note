#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#define MAX 10
int number;

void* funcA_num(void* arg) {
    // 随便做一些事情，以此来增加cpu时间片的争夺
    for(int i=0; i<MAX; ++i) {
        int cur = number;
        cur++;
        usleep(10);
        number = cur;
        printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
    }

    return NULL;
}

void* funcB_num(void* arg) {
    for(int i=0; i<MAX; ++i) {
        int cur = number;
        cur++;
        number = cur;
        printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
        usleep(5);
    }
    return NULL;
}

int main(int argc, const char* argv[]) {
    pthread_t p1, p2;

    pthread_create(&p1, NULL, funcA_num, NULL);
    pthread_create(&p2, NULL, funcB_num, NULL);
    // 创建两个子线程，让他们同时使用同一个全局变量

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
/*
运行结果：
Thread B, id = 123148508849728, number = 1
Thread B, id = 123148508849728, number = 2
Thread A, id = 123148517242432, number = 1
Thread B, id = 123148508849728, number = 2
Thread B, id = 123148508849728, number = 3
Thread A, id = 123148517242432, number = 2
Thread B, id = 123148508849728, number = 3
Thread A, id = 123148517242432, number = 3
Thread B, id = 123148508849728, number = 4
Thread A, id = 123148517242432, number = 4
Thread A, id = 123148517242432, number = 5
Thread A, id = 123148517242432, number = 6
Thread A, id = 123148517242432, number = 7
Thread A, id = 123148517242432, number = 8
Thread A, id = 123148517242432, number = 9
Thread B, id = 123148508849728, number = 10
Thread A, id = 123148517242432, number = 10
Thread B, id = 123148508849728, number = 11
Thread B, id = 123148508849728, number = 12
Thread B, id = 123148508849728, number = 13
*/
