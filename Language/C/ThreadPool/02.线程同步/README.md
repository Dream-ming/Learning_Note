## 线程同步

情景举例 0.无线程同步.c  

当A在使用cpu时间片的时候，用cur获取了number的值并自加，还未写回number时，  
B拿到了下一个时间片，于是B重复拿到了一个number，并且进行了自加和输出，发生数据紊乱

## 互斥锁(pthread_mutex_t)

互斥锁可以锁定一段代码块  
被锁定的代码块只能顺序执行，不能并行。以降低效率的代价，保证数据正常不紊乱。

使用方式：
* pthread_mutex_t mutex：定义一个名为mutex的互斥锁
* pthread_mutex_init(&mutex, NULL)：初始化互斥锁，使用默认属性NULL
* pthread_mutex_destroy(&mutex)：销毁互斥锁，使用完释放资源
* pthread_mutex_lock(&mutex)：加锁，如果该锁被使用，则线程变成阻塞状态，直到可以使用该锁，执行完代码段为止
* pthread_mutex_trylock(&mutex)：尝试加锁，如果该锁被使用，则先执行其他步骤
* pthread_mutex_unlock(&mutex)：解锁，使用完锁，一定要解锁

## 死锁

死锁是指上锁不当导致程序被无限阻塞的情况

死锁出现的四个必要条件：    
(1) 互斥条件：一个资源每次只能被一个进程使用。  
(2) 请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。  
(3) 不剥夺条件:进程已获得的资源，在末使用完之前，不能强行剥夺。  
(4) 循环等待条件:若干进程之间形成一种头尾相接的循环等待资源关系。  
这四个条件是死锁的必要条件，只要系统发生死锁，这些条件必然成立，而只要上述条件之一不满足，就不会发生死锁。  

举例
* 上锁后忘记解锁
* 在上锁后，还未解锁又进行一次上锁
* A锁上资源1,B锁上资源2,A执行过程中需要资源2,B执行过程中需要资源1,双方无限等待

## 读写锁(pthread_rwlock_t)

读写锁是互斥锁的升级版，将锁分为读操作锁和写操作锁  
性质：  
1、当多个线程对同一个资源进行读操作时可以并行  
2、当多个线程对同一个资源进行写操作时互斥，需要串行  
3、既有读又有写时，写操作优先，等写操作串行全部完成，读操作并行进行  

使用方式：  
* pthread_rwlock_t rwlock：定义一个读写锁
* pthread_rwlock_init(&rwlock, NULL)：初始化，使用默认属性NULL
* pthread_rwlock_destroy(&rwlock)：销毁
* pthread_rwlock_rdlock(&rwlock)：上一个读锁，如果该锁锁定写，则等待。如果该锁为空或被其他线程上了读锁，则可以执行读操作
* pthread_rwlock_wrlock(&rwlock)：上一个写锁，等待锁空，上锁使用
* pthread_rwlock_tryrdlock(&rwlock)：尝试上一个读锁
* pthread_rwlock_trywrlock(&rwlock)：尝试上一个写锁
* pthread_rwlock_unlock(&rwlock)：解锁，读锁和写锁实际上是针对一个锁而言，所以直接解锁即可，不分读写

## 条件变量(pthread_cond_t)

条件变量通常和互斥锁一起使用，实现生产者消费者模型  

使用方法：
* pthread_cond_t cond：定义一个条件变量
* pthread_cond_init(&cond, NULL)：初始化，使用默认属性NULL
* pthread_cond_destroy(&cond)：销毁
* pthread_cond_signal(&cond)：唤醒堵塞在 cond 上的至少一个线程
* pthread_cond_broadcast(&cond)：唤醒所有堵塞在 cond 上的线程
* pthread_cond_wait(&cond, &mutex)：被 cond 堵塞，暂时解开 mutex ，等待 signal 或者 broadcast 唤醒

## 信号量

类似条件变量，与互斥锁一起解决生产者消费者模型

需要头文件<semaphore.h>

使用方式：
* sem_t sem：定义一个信号量
* sem_init(&sem, 0, num)：初始化信号量，第二个参数0表示进程同步，其他值表示线程同步，第三个参数表示初始时 sem 有 num 个资源
* sem_destroy(&sem)：销毁
* sem_wait(&sem)：等待 sem 资源，如果有则消耗一个，资源数减一，否则持续等待
* sem_trywait(&sem)：尝试获取资源，如果有则消耗一个，资源数减一，否则返回错误号
* sem_post(&sem)：给 sem 增加一个资源
* sem_getvalue(&sem, &num)：查询 sem 里的资源个数，返回 num
