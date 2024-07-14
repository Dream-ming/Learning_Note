# C语言学习笔记

## 1、sscanf 分割字符串

将字符串分割为想要的格式

使用正则表达式分割，%作为分隔符
* \[1-9]: 取出数字
* \[a-z]: 取出小写字母
* \[^A-Z]: 一直取，直到出现大写字母
* %\[^:]://%\[^:]:%\[1-9]: 一直取直到碰到冒号，跳过://一直取直到碰到冒号，跳过:取出后面所有的数字

```c++
const char *s = "http://www.baidu.com:1234
char protocol[32] = {0};
char host[128] = {0};
char port[8] = {0};
sscanf(s, "%[^:]://%[^:]:%[1-9]", protocol, host, port);
// protocol: http
// host: www.baidu.com
// port: 1234
```

## 2、scandir 遍历目录

使用 scandir() 函数进行目录的遍历

```c
#include <dirnet.h>
int scandir(const char* dirp, struct dirent*** namelist,
                int (*filter)(const struct dirent *),
                int (*compar)(const struct dirent **, const struct dirent **));
// 第一个参数 dirp : 需要遍历的目录的名字
// 第二个参数 namelist : 三级指针，传出参数，指定地址传出得到的信息
// 第三个参数 filter : 函数指针，自定义筛选目录的条件，在函数中需要的目录返回1，不需要的返回0
// 第四个参数 compar : 函数指针，自定义文件顺序，可以使用系统的函数alphasort(根据文件名排序)，versionsort(根据版本排序)
```

## 3、sendfile 发送文件函数

```c
off_t offset = 0; // off_t本质是一个整形
int size = lseek(fd, 0, SEEK_END); // 得到文件的字节数，此处lseek会把指针移动到文件尾部
lseek(fd, 0, SEEK_SET); // 把文件指针移回头部，让sendfile能从文件头部开始发送
while (offset < size) // sendfile有文件大小的限制，如果发送文件太大了，就会只发送一部分
{
    int ret = sendfile(cfd, fd, &offset, size - offset); // 客户端，本地文件描述符，文件偏移量，文件大小
    // 如果一次没有发送完，offset会移动到当前发送的位置，下次再调用时指定该偏移量即可继续发送
    printf("ret value: %d\n", ret); // 如果成功发送，返回发送的字节数
    if (ret == -1)               // 如果发送失败，返回-1
    {
        perror("sendfile");
    }
}
```
