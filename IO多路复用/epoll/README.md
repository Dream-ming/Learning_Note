# epoll

仅限Linux使用   
基于红黑树处理待检测的文件描述符，使用回调机制，与用户区共享内存减少拷贝开销，是io多路复用的不二之选~

用法：   
* #include <sys/epoll.h>
* int epoll_create(int size);
  * 创建一个 epoll 实例
  * 第一个参数 size
    * 在Linux内核2.6.8以前，指的是维护的最大文件描述符个数
    * 在2.6.8以后，是一个无效的参数，只需要指定一个大于0的数即可
  * 创建成功返回一个文件描述符，通过这个描述符可以访问epoll实例，失败返回-1

```c++
// 联合体, 多个变量共用同一块内存        
typedef union epoll_data {
    void        *ptr;
    int          fd;	// 通常情况下使用这个成员, 和epoll_ctl的第三个参数相同即可
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};
```

* int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
  * 管理红黑树上的节点，可以进行增、删、改操作
  * 第一个参数 epfd
    * 是epoll_create的返回值，指向一个epoll实例
  * 第二个参数 op
    * 是一个枚举值，指定改函数执行的操作
    * EPOLL_CTL_ADD：在epoll模型中添加新的节点
    * EPOLL_CTL_MOD：修改epoll中已经存在的节点
    * EPOLL_CTL_DEL：删除epoll模型中的指定节点
  * 第三个参数 fd
    * 文件描述符，即要添加、修改、删除的文件描述符
  * 第四个参数 *event
    * 传入一个epoll_event结构体类型
    * 结构体中第一个参数 events 用于指定检测的事件
      * EPOLLIN: 检测读缓冲区是否有数据
      * EPOLLOUT: 检测写缓冲区是否可写
      * EPOLLERR: 检测是否异常
      * EPOLLET: 边沿触发模式，默认为水平触发模式
    * 结构体中的第二个参数是一个联合体
      * 用户可以自行传入一些参数，内核不做修改，原样传出
      * 比如传入文件描述符，这样拿到这个节点的时候就知道对应哪个了
* int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
  * 阻塞函数，检测 epoll 树上是否有可用的文件描述符
  * 第一个参数 epfd
    * epoll_create的返回值，指向epoll实例
  * 第二个参数 *event
    * 传出参数
    * 当有文件描述符就绪时，传出到此结构体
  * 第三个参数 maxevents
    * 修饰第二个参数，指定最大容量
  * 第四个参数 timeout
    * 指定检测的时间(毫秒)
    * 指定-1会永久检测
  * 返回值
    * 已经就绪的文件描述符的个数
