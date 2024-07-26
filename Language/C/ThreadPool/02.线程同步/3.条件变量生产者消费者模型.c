#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

struct Node {
    int number;
    struct Node* next;  
};
Node* head = NULL;

void* producer(void* arg) {

    while(1) {
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("生产者, id: %ld, number: %d\n", pthread_self(), newNode->number);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
        sleep(rand() % 3);
    }

    return NULL;
}

void* consumer(void* arg) {

    while (1) {
        pthread_mutex_lock(&mutex);
        while (head == NULL) {
            // 必须写 while ，而不能是 if
            // 当多个线程在此被堵塞时，唤醒后，只有一个线程能抢到时间片
            // 其他的线程继续判断是否还有资源
            pthread_cond_wait(&cond, &mutex);
            // wait，把mutex打开
        }
        struct Node* node = head;
        printf("消费者,id: %ld, number: %d\n", pthread_self(), node->number);
        head = head->next;
        free(node);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);
    }

    return NULL;
}


int main() {

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t t1[5], t2[5];

    for (int i = 0; i < 5; ++i) {
        pthread_create(&t1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}
