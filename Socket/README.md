# 套接字通信基础知识

这里的笔记是Linux的套接字用法，window略有不同但大差不差，要用到的时候再去找

## 前置知识

字节序分为大端(网络字节序)和小端(主机字节序)

举例：0x12345678 -> 16进制数，共占四个字节    
* 大端：套接字通信过程中数据存储方式，低字节存储在内存的低位地址，即 12 34 56 78
* 小端：PC机数据默认的存储方式，低位字节存储在内存的高位地址，即 78 56 34 12

### 大小端转换函数
* #include <arpa/inet.h>
* // u: unsigned, 16: 16位, 32: 32位, s: short, l: int
* // h: host(主机字节序), n: net(网络字节序)
* uint16_t htons(uint16_t hostshort);	 // short类型的大端转小端
* uint32_t htonl(uint32_t hostlong);	 // int类型的大端转小端
* uint16_t ntohs(uint16_t netshort)    // short类型的小端转大端
* uint32_t ntohl(uint32_t netlong);    // int类型的小端转大端

### IP地址转换

大小端转换函数仅针对数值型，一般存ip还会有.用于分隔，inet_pton和inet_ntop处理

* #include <arpa/inet.h>
* int inet_pton(int af, const char *src, void *dst);
    * 第一个参数 af
        *  AF_INET: ipv4
        *  AF_INET6: ipv6
    * 第二个参数 src
        * 传入参数，小端点分十进制ip地址
    * 第三个参数 dst
        * 传出参数，得到大端整形的ip
    *  成功返回1，失败返回0或-1
* const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
    * af 同上
    * src: 传入参数，大端的整形ip
    * dst: 传出参数，小端点分十进制ip地址
    * size: 修饰dst参数，标记dst指向的内存中最多可以存储多少个字节
    * 成功返回第三个参数对应的内存地址，失败NULL

### sockaddr, in_addr, sockaddr_in

三个结构体，用于socket函数的传参，存储的是协议、ip和端口

```c++
// 在写数据的时候不好用，一般是定义一个sockaddr_in，分别给ip和port赋值，然后转换为sockaddr
struct sockaddr {
	sa_family_t sa_family;       // 地址族协议, ipv4
	char        sa_data[14];     // 端口(2字节) + IP地址(4字节) + 填充(8字节)
}

struct in_addr
{
    in_addr_t s_addr;  // 本质是一个数值类型
};  

// sizeof(struct sockaddr) == sizeof(struct sockaddr_in)
struct sockaddr_in
{
    sa_family_t sin_family;		/* 地址族协议: AF_INET */
    in_port_t sin_port;         /* 端口, 2字节-> 大端  */
    struct in_addr sin_addr;    /* IP地址, 4字节 -> 大端  */
    /* 填充 8字节 */
    unsigned char sin_zero[sizeof (struct sockaddr) - sizeof(sin_family) -
               sizeof (in_port_t) - sizeof (struct in_addr)];
}; 
// 使用时定义一个sockaddr_in，分别给sin_family, sin_prot, sin_addr赋值
// 然后(struct addr*)&sockaddr_in，强制转换即可
```

## 通信流程

[![f74602e1d3f9221982fee7ed0c69ffd.png](https://i.postimg.cc/cJZY72hH/f74602e1d3f9221982fee7ed0c69ffd.png)](https://postimg.cc/hXyfcCHW)

## socket 和 bind
  
* #include <sys/types.h>
* #include <sys/socket.h>
* int socket(int domain, int type, int protocal);
    * 创建一个套接字
    * 第一个参数 domain ，指定ip协议
        * AF_INET: ipv4
        * AF_INET6: ipv6
        * AF_UNIX, AF_LOCAL: 本机通信，同一台机器的进程之间
    * 第二个参数 type ，指定传输层协议
        * SOCK_STREAM: 流式传输协议，TCP
        * SOCK_DGRAM: 报式传输协议，UDP
    * 第三个参数 protocal
        * 写0即可，使用默认的协议
    * 成功返回一个套接字(用于监听的文件描述符)，失败返回-1
* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    * 将文件描述符和本地的IP与端口绑定
    * 第一个参数 sockfd
        * 用于监听的文件描述符，通过socket()调用得到
    * 第二个参数 addr
        * 传入要绑定的IP和端口信息，必须是大端，函数会给addr赋值
    * 第三个参数 addrlen
        * 传入 addr 的内存大小，sizeof(struct sockaddr)即可
    * 成功返回0，失败返回-1

## listen 和 accept

* #include <sys/types.h>
* #include <sys/socket.h>
* int listen(int sockfd, int backlog);
    * 用于监听有没有新的客户端连接
    * 第一个参数 sockfd
        * 用于监听的文件描述符，由socket()调用得到
    * 第二个参数 backlog
        * 设置同时处理的最大连接要求，最大值位128
    * 函数调用成功返回0，失败返回-1
* int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    * 等待并接受客户端的连接请求，建立新的连接会得到一个新的文件描述符
    * 是一个阻塞函数，如果没有新的客户端连接，就会一直阻塞
    * 第一个参数 sockfd，由socket()得到
    * 第二个参数 addr
        * 传出参数，存储了建立连接的客户端的地址信息
        * 如果不需要，指定为空即可
    * 第三个参数 addrlen
        * 传入传出参数
        * 如果addr指定了有效内存，就需要给addrlen传入addr的大小(sizeof)，会传出修改后的内存大小
        * 如果不需要，指定为空
    * 调用成功返回一个用于通信的文件描述符，失败返回-1
 
## connect
* #include <sys/types.h>
* #include <sys/socket.h>
* int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    * 用于客户端连接服务器
    * 第一个参数 sockfd
        * 通信的文件描述符，socket()得到
    * 第二个参数 addr
        * 存储了要连接的服务器端的IP和端口，需要大端
    * 第三个参数 addrlen
        * sizeof(struct addr);
    * 连接成功返回0，失败返回-1
 
## recv 和 send
* #include <sys/types.h>
* #include <sys/socket.h>
* ssize_t recv(int sockfd, void *buf, size_t size, int flags);
   * 如果连接没有断开，接收端没有收到数据，函数就会阻塞等待数据
   * 第一个参数 socket
      * 通信的文件描述符，accept()函数的返回值
   * 第二个参数 *buf
      * 指向一块有效内存，用于存储接受的数据
   * 第三个参数 size
      * 参数buf指向的内存的容量
   * 第四个参数 flags
      * 特殊的属性，一般不使用，指定为0
   * 返回值，大于0实际接受的字节数，等于0对方断开了连接，等于-1接受数据失败
* ssize_t send(int fd, const void *buf, size_t len, int flags);
   * 发送数据的函数
   * 第一个参数 fd
      * 通信的文件描述符，accept()函数的返回值
   * 第二个参数 *buf
      * 传入参数，要发送的字符串
   * 第三个参数 len
      * 传入参数，要发送的字符串的长度
   * 第四个参数 flags
      * 特殊的属性，一般不使用，指定为0
   * 返回值，大于0实际发送的字节数，等于-1发送数据失败

## read 和 write
* #include <unistd.h>
* ssize_t read(int sockfd, void *buf, size_t size);
   * 接受数据函数，用法同recv，在不同的头文件下，不需要第四个参数
* ssize_t write(int fd, const void *buf, size_t, len);
   * 发送数据函数，用法同send，在不同的头文件下，不需要第四个参数

## close
* #include <unistd.h>
* int close(int fd);
   * 关闭套接字fd
