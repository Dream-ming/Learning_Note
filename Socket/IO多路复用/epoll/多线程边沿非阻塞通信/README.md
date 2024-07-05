# epoll 的多线程边沿非阻塞通信

## 边沿触发模式(ET)

边沿触发模式，epoll_wait仅在第一次发现数据的时候传递给用户，所以用户要一次性读完数据   

设置ET模式，在epoll_event结构体中写读写事件的时候带上 EPOLLET 即可

```c++
struct epoll_event ev;
ev.events = EPOLLIN | EPOLLET;
```

## 非阻塞

用 ET 模式接受数据必须在第一次得到套接字的时候就全部处理

如果开一个很大的数组，一次性read的话，浪费内存   
选择用循环接受数据，但是循环调用read的时候，read会阻塞线程，导致无法处理其他操作   
所以要把套接字修改为非阻塞的   

```c++
#include<fcntl.h>  // 使用 fcntl 需要头文件
// 设置完成之后, 读写都变成了非阻塞模式
int flag = fcntl(cfd, F_GETFL);
flag |= O_NONBLOCK;                                                        
fcntl(cfd, F_SETFL, flag);
```

while(1)循环调用read/recv函数把数据读入，怎么判断读入完毕呢？   
当缓冲区数据读完，再调用read就会返回-1，并且全局变量 errno 值会变为 EAGAIN   

```c++
#include <errno.h>  // 使用 errno 需要头文件
int len = recv(curfd, buf, sizeof(buf), 0);
if(len == -1)
{
    if(errno == EAGAIN)
    {
        printf("数据读完了...\n");
    }
    else
    {
        perror("recv");
        exit(0);
    }
}
```

## 本项目注意点

这里实现了 epoll 的多线程边沿触发非阻塞操作

有两个点：

一是：epoll_wait 每发现一个套接字就创建一个新的线程，比较浪费时间，可以用线程池优化

二是：监听的套接字 lfd 没有设置 ET 模式   
当一个客户端发起连接时，epollwait 发现数据，创建一个线程去处理，但是创建线程比较慢，epollwait发现数据没有被处理，又重复创建线程去处理这个数据
