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

    printf("主线程：%ld\n", pthread_self());

    pthread_detach(tid);
    // 将 tid 线程和主线程分离
    pthread_exit(NULL);
    // 结束主线程，tid 仍然会继续
    return 0;
}
