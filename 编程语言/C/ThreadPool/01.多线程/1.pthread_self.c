#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main() {
    printf("主线程: %ld\n", pthread_self());
    // 输出当前线程的ID
    return 0;
}
/*
头文件<pthread.h>，在编译文件的时候，需要指定动态库-l pthread
pthread_t是线程id类型
pthread_create创建一个子线程，则会把当前线程当成主线程
*/
