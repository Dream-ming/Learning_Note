#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct EE{
    int num;
};
void* callback(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("子线程:i = %d\n", i);
    }
    printf("%ld\n", pthread_self());
    
    struct EE* t = (struct EE*) arg;
    t->num = 100;
    pthread_exit(t);
    return NULL;

}

int main() {
    struct EE t;
    pthread_t tid;
    pthread_create(&tid, NULL, callback, &t);
    // 创建线程的时候，传一个结构体变量给子线程
    // 相当于让子线程使用主线程的栈区

    printf("主线程：%ld\n", pthread_self());

    void* ptr;
    pthread_join(tid, &ptr);
    // t 在子线程中被修改过，但由于内容在主线程的栈区
    // 直接输出即可
    // 做到回收子线程内容的效果
    printf("num = %d\n", t.num);

    return 0;
}
