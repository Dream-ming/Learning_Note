### pthread_t
线程 ID 变量类型

### pthread_self()
查询当前线程的 ID 

### pthread_create(pthread_t *, const pthread_attr_t \*attr, void\*, void *arg)
创建线程函数    

需要传四个参数：线程ID，线程的属性(一般使用默认属性，写NULL)，函数指针(一个函数名，该函数在本子线程中执行)，传递给函数的参数

### pthread_exit(NULL)
退出线程，返回NULL，也是子线程返回值给主线程接受

### pthread_join(pthread_t, void **val)
回收线程，如果子线程在执行，就会被阻塞

val接受子线程exit返回的值

### pthread_detach(pthread_t)
将子线程和主线程分离，退出主线程后，子线程仍然可执行
