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
