# IO多路复用

## Socket多线程操作

[![7132cab2bbfa070f4bfaf61efa7a54c.png](https://i.postimg.cc/yxW1N9cd/7132cab2bbfa070f4bfaf61efa7a54c.png)](https://postimg.cc/LgKKCJyK)

在Socket通信中，服务端有两类文件描述符，一个用于监听的，N个用于通信的

监听描述符的读缓冲区会等待客户端的connect请求，accept检测到读缓冲区有数据就会读出数据建立连接，否则一直阻塞

通信描述符的读缓冲区会等待客户端的send或者write请求，read或recv检测到读缓冲区有数据就会读出来，否则一直阻塞

通信描述符的写缓冲区，write或send操作，当写缓冲区有位置时，就写入并发送给客户端，否则阻塞

客户端的读写缓冲区操作类似

如果只有单线程，那么上述的阻塞会相互影响，read阻塞了就无法write或accept，于是有了多线程的Socket

IO多路复用：把检测文件描述符的读写缓冲区的操作交给内核，系统不必创建进程/线程，降低了系统开销

## Select函数

select函数支持跨平台Linux、Mac、Windows

用法：
```c++
#include <sys/select.h>
struct timeval {
    time_t      tv_sec;         /* seconds */
    suseconds_t tv_usec;        /* microseconds */
};

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval * timeout);

// fd_set类型的操作

// 将文件描述符fd从set集合中删除 == 将fd对应的标志位设置为0        
void FD_CLR(int fd, fd_set *set);

// 将文件描述符fd添加到set集合中 == 将fd对应的标志位设置为1
void FD_SET(int fd, fd_set *set);

// 判断文件描述符fd是否在set集合中 == 读一下fd对应的标志位到底是0还是1
int  FD_ISSET(int fd, fd_set *set);

// 将set集合中, 所有文件文件描述符对应的标志位设置为0, 集合中没有添加任何文件描述符
void FD_ZERO(fd_set *set);
```
* 第一个参数 nfds
  * 委托内核检测的文件描述符个数+1
  * select的本质是内核通过线性表遍历每一个文件描述符，所以要传递个数
  * 最大为1024
* 第二个参数 fd_set *readfds
  * 传入传出参数，文件描述符集合
  * 内核只检测这个集合中文件描述符的读缓冲区
  * 如果读缓冲区可用，内核会把文件描述符写回这个地址
* 第三个参数 fd_set *writedfs
  * 传入传出文件描述符，内核检测写缓冲区
  * 同 readfds
* 第四个参数 fd_set *exceptfds
  * 传入传出参数，把异常的fd写在这里
* 第五个参数 struct timeval * timeout
  * 传入一个时间，超过这个时间还没有检测到可用的文件描述符，就先返回
  * 结构体内部一个秒，一个微秒，记得初始化
  * 传入NULL时，看作无限时间，直到有文件描述符处于就绪状态为止
* 函数返回值
  * 大于0：成功，返回集合中已就绪的文件描述符的总个数
  * 等于0：超时，没有检测到就绪的文件描述符
  * 等于-1：函数调用失败
* 函数细节
  * 调用函数的时候传入1~3个fd_set给内核指定要检测的文件描述符(最大为1024)
  * 内核将fd_set对应到内部的文件描述符表检测读、写或异常，然后重新将数值写回到fd_set中
