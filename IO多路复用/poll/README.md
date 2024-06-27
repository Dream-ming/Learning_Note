# poll

在Linux下可以使用select、poll、epoll

select 和 poll 底层处理逻辑一模一样，但是select支持多平台

综上，一般都不使用poll，这里做简单的了解

```c++
#include <poll.h>
// 每个委托poll检测的fd都对应这样一个结构体
struct pollfd {
    int   fd;         /* 委托内核检测的文件描述符 */
    short events;     /* 委托内核检测文件描述符的什么事件 */
    short revents;    /* 文件描述符实际发生的事件 -> 传出 */
};
struct pollfd myfd[100];

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```
* #include <poll.h>
* int poll(struct pollfd *fds, nfds_t nfds, int timeout);
* 第一个参数 *fds
  * pollfd结构体，内部有fd、传入参数、传出参数
  * events可以指定POLLIN、POLLOUT，检测读、写缓冲区，或者POLLIN | POLLOUT，同时检测
  * revents传出检测后，有读、写、读或写的文件描述符
* 第二个参数 nfds
  * 指定个数
  * 数组中最后一个有效元素的下标+1，或者直接数组的总元素个数
* 第三个参数 timeout
  * 规定时间，检测指定时间后返回，单位毫秒
  * -1：无限阻塞，直到有文件描述符就绪
  * 0，不阻塞，检测完毕就返回
  * 大于0，阻塞指定的毫秒数
  * 返回值
    * 失败：返回-1
    * 成功：返回一个大于0的整数，表示检测的集合中已就绪的文件描述符的总个数
