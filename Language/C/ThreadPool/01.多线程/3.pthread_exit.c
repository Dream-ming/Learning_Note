#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* callback(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("子线程:i = %d\n", i);
    }
    printf("%ld\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    printf("主线程: %ld\n", pthread_self());
    pthread_exit(NULL);
    // 将主线程退出了
    // 但是子线程的线程函数会继续输出for循环内容
    return 0;
}
