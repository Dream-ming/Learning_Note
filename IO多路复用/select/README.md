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
