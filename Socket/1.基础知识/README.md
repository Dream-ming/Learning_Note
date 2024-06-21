# 套接字通信基础知识

## 字节序

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
* #include <arpa/inet.h>
* int inet_pton(int af, const char *src, void *dst);
  *  AF_INET: ipv4
  *  AF_INET6: ipv6
  *  src: 传入参数，小端点分十进制ip地址
  *  dst: 传出参数，得到大端整形的ip
  *  成功返回1，失败返回0或-1
* const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
  * af 同上
  * src: 传入参数，大端的整形ip
  * dst: 传出参数，小端点分十进制ip地址
  * size: 修饰dst参数，标记dst指向的内存中最多可以存储多少个字节
  * 成功返回第三个参数对应的内存地址，失败NULL

## 通信流程

[![f74602e1d3f9221982fee7ed0c69ffd.png](https://i.postimg.cc/cJZY72hH/f74602e1d3f9221982fee7ed0c69ffd.png)](https://postimg.cc/hXyfcCHW)
